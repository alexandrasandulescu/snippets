#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s size\n", argv[0]);
		return -1;
	}

	int size = atoi(argv[1]);
	unsigned long address = (unsigned long) mmap(0, size,
			PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	printf("Received address %08lx for size %d\n", address, size);
	return 0;
}
