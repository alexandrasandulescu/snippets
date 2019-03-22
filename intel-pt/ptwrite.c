#include <stdio.h>
#include <stdlib.h>
#include <cpuid.h>
#include <stdint.h>
#include <x86intrin.h>

#define CPUID 1
#define LEAF 2
#define PTWRITE 3

int has_ptwrite() {
	unsigned int eax, ebx, ecx, edx;
	
	if (__get_cpuid(0x00, &eax, &ebx, &ecx, &edx) == 0) {
		return -CPUID;
	}
	if (eax < 0x14) {
		return -LEAF;
	}
	__cpuid_count(0x14, 0x00, eax, ebx, ecx, edx);
	if ((ebx & 0x10) == 0) {
		return -PTWRITE;
	}
	return 0;
}

int main() {
	int ret = has_ptwrite();
	char *msg;
	switch(ret) {
		case -CPUID:
			msg = "CPUID";
			break;
		case -LEAF:
			msg = "LEAF 0x14";
			break;
		case -PTWRITE:
			msg = "PTWRITE";
			break;
		default:
			return 0;
	}
	printf("%s not supported\n", msg);
	uint64_t data = 0xdeadbeefdeadbeef;
//	_ptwrite64(data);
	unsigned nr = 0xdeadbeef;
	unsigned rot = 1;
	asm (
		"rorl %1, 8\n\t"
		: "+g" (nr)
		: //"cI" ((unsigned char)rot)
	);
	return 0;
}
