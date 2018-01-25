
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: %s abs_path\n", argv[0]);
		exit(-1);
	}
	printf("%s\n", basename(argv[1]));
	return 0;
}
