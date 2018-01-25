#include <stdio.h>
int main() {
	struct a {
		unsigned int x;
		unsigned short y;
		unsigned short z;
		unsigned short t;
	};

	printf("%lu\n", sizeof(struct a));
}
