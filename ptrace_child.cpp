#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>

//#define ORIG_EAX 11

struct pt_regs {
	long ebx;
	long ecx;
	long edx;
	long esi;
	long edi;
	long ebp;
	long eax;
	int  xds;
	int  xes;
	int  xfs;
	int  xgs;
	long orig_eax;
	long eip;
	int  xcs;
	long eflags;
	long esp;
	int  xss;
};



int main()
{   pid_t child;
	int status;
	int insyscall = 0;
	long params[3];
	long orig_eax, eax;
	struct pt_regs regs;

	char *arg = "./tested";
	child = fork();
	if(child == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execve(arg, &arg, NULL);
	}

	else {
		while(1) {
			wait(&status);
			if(WIFEXITED(status))
				break;
			ptrace(PTRACE_GETREGS, child, 0, &regs);
			printf("Child started. EIP = 0x%08lx\n", regs.eip);
			ptrace(PTRACE_CONT, child, 0, 0);

		}
	}
	return 0;
}

