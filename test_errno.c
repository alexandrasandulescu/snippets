#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dlfcn.h>

#include <inttypes.h>


int main(int argc, char **argv) {
	if(argc < 2) {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	void *libc = dlopen("libc.so", RTLD_LAZY);
	if (libc == NULL) {
		printf("Cannot find libc.so\n");
		exit(1);
	}

	void *local___errno_location = dlsym(libc, "__errno_location");
	if (local___errno_location == NULL) {
		printf("Cannot find __errno_location\n");
		exit(1);
	}

	uint32_t libc_address = *((uint32_t *)libc);
	printf("%08x %p %08x\n", libc_address, local___errno_location,
			((uint32_t)local___errno_location - libc_address));

	int ret = open(argv[1], O_RDONLY);
	if (ret == -1) {
		printf("Errno set to %d\n", errno);
		exit(1);
	}

	return 0;
}
