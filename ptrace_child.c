#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h> 
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "os-linux.h"

const int long_size = sizeof(long);

void getdata(pid_t child, long addr,
		char *str, int len)
{   char *laddr;
	int i, j;
	union u {
		long val;
		char chars[long_size];
	}data;
	i = 0;
	j = len / long_size;
	laddr = str;
	while(i < j) {
		data.val = ptrace(PTRACE_PEEKDATA, child,
				addr + i * 4, NULL);
		memcpy(laddr, data.chars, long_size);
		++i;
		laddr += long_size;
	}
	j = len % long_size;
	if(j != 0) {
		data.val = ptrace(PTRACE_PEEKDATA, child,
				addr + i * 4, NULL);
		memcpy(laddr, data.chars, j);
	}
	str[len] = '\0';
}
void putdata(pid_t child, long addr,
		char *str, int len)
{   char *laddr;
	int i, j;
	union u {
		long val;
		char chars[long_size];
	}data;
	i = 0;
	j = len / long_size;
	laddr = str;
	while(i < j) {
		memcpy(data.chars, laddr, long_size);
		ptrace(PTRACE_POKEDATA, child,
				addr + i * 4, data.val);
		++i;
		laddr += long_size;
	}
	j = len % long_size;
	if(j != 0) {
		memcpy(data.chars, laddr, j);
		ptrace(PTRACE_POKEDATA, child,
				addr + i * 4, data.val);
	}
}

unsigned int get_entry0(char *elf_path) {
	return 0x8048310;
}

unsigned int ld_loader_address(pid_t pid) {
	struct map_iterator mi;
	struct map_prot mp;
	unsigned long hi;
	unsigned long segbase, mapoff;


	if (maps_init (&mi, pid) < 0) {
		printf("Cannot find maps for pid %d\n", pid);
		return 0;
	}

	while (maps_next (&mi, &segbase, &hi, &mapoff, &mp)) {
		if (0 == strcmp(mi.path, "/home/alex/snippets/preload_child.so")) {
			printf("Found child mapping for ld_preload %p\n", (void*)segbase);
			maps_close(&mi);
			return segbase;
		}

	}
	maps_close(&mi);
	return 0;
}

int shmAlloc;

int  InitializeAllocator() {
	shmAlloc = shm_open("/thug_life", O_CREAT | O_RDWR | O_TRUNC | O_EXCL, 0644);
	if (shmAlloc == -1) {
		printf("Could not allocate shared memory chunk. Exiting. %d\n", errno);
		strerror(errno);
		return 0;
	}

	printf("[Parent] Opened shared mem @fd%d\n", shmAlloc);
	return 1;
}



int main()
{   pid_t child;
	int status;
	struct user_regs_struct regs;

	/* int 0x80, int3 */
	char code[] = {0xcd,0x80,0xcc,0};
	char backup[4];
	char bad_code[] = {0xde, 0xad, 0x90, 0x90};

	char *arg = "./tested";
	char *const args[] = {"./tested", NULL};
	char *const envs[] = {"LD_PRELOAD=/home/alex/snippets/preload_child.so", NULL};

	InitializeAllocator();
	unsigned long address = 0;
	child = fork();
	if(child == 0) {
		int ret = ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		if (ret)
			printf("Ptrace traceme failed with exit code %d\n", ret);

		ret = execve(arg, args, envs);
		if (ret ==-1)
			printf("Execve failed\n");
	}

	else {
		unsigned addr = get_entry0(arg); //get entry0
		wait(&status);

		ptrace(PTRACE_GETREGS, child, 0, &regs);
		printf("Child started. EIP = 0x%08lx\n", regs.eip);
		fflush(stdout);
		//take 3 bytes from eip
		getdata(child, addr, backup, 3);
		putdata(child, addr, code, 3);

		//continue
		ptrace(PTRACE_CONT, child, NULL, NULL);
		wait(NULL);

		//read address symbol from libloader.so; get library mapping address
		unsigned long symbolOffset = 0x2014;
		unsigned long symbolAddress = ld_loader_address(child) + symbolOffset;
		char sym_addr[4] = {0, 0, 0, 0};
		getdata(child, symbolAddress, sym_addr, 4);

		for (int i = 0; i < 4; ++i) {
			((char*)&address)[i] = sym_addr[i];
		}
		printf("Translated shared memory address to %lx\n", address);
		//should be in main
		ptrace(PTRACE_GETREGS, child, 0, &regs);
		printf("Child should be in main. EIP = 0x%08lx\n", regs.eip);

		fflush(stdout);
		putdata(child, regs.eip, backup, 3);

		//try to load .so in here

		address = (unsigned long)mmap((void*)address, 1 << 30, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, shmAlloc, 0);
		strerror(errno);
		printf("In parent adddress is %lx\n", address);

		sleep(20);
		long ret = ptrace(PTRACE_CONT, child, NULL, NULL);
		wait(&status);
		if (WIFEXITED(status) ) {
			int es = WEXITSTATUS(status);
			printf("Exit status was %d\n", es);
		}
		fflush(stdout);

		ret = ptrace (PTRACE_DETACH, child, NULL, NULL);


	}
	shm_unlink("/thug_life");
	return 0;
}

