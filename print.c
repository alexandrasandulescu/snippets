
#include <stdio.h>

int main () {
	int printed;
	char buf[32];
	printf("%d\n", sizeof(long unsigned int));
	printf("%llx%n", 12, &printed);
	printf("printed: %d\n", printed);
	return 0;
}
