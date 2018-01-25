#include <stdio.h>

int __attribute__((__stdcall__)) add(int a, int b) {
	return a + b;
}

int main(int argc, char **argv) {
	printf("%d\n", add(argc, argc));
	return 0;
}
