#include <stdio.h>
int main() {
	int a  = 5;

	printf("1: %d\n", a);
	if (a >= 5) {
		int a = 2;
		printf("2: %d\n", a);
	}
	printf("3: %d\n", a);
}
