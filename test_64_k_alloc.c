#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>


size_t lastheapsize = 0, heapsize = 0;
void get_proc() {
	FILE * f = fopen("/proc/self/maps", "r");
	assert(f != NULL);
	char *line = NULL;
	size_t len = 0, read;


	while ((read = getline(&line, &len, f)) != -1) {
		if (strstr(line, "[heap]") != NULL) {
			char *fst = strchr(line, '-');
			assert(fst != NULL);
			fst[0] = '\0';
			unsigned int start = strtol(line, NULL, 16);
			char *snd = strchr(fst + 1, ' ');
			assert(snd != NULL);
			snd[0] = '\0';
			unsigned int end = strtol(fst + 1, NULL, 16);
			lastheapsize = heapsize;
			heapsize = (end - start) / 1024;
			printf("Heap size is: %u K [%d]\n", heapsize, (int)heapsize - (int)lastheapsize);
			free(line);
			break;
		}
		free(line);
		line = NULL;
		len = 0;
	}
	fclose(f);
}

int main() {
	size_t size = 100;
	unsigned int addrs[size];
	for (int i = 0; i < size; ++i) {
		addrs[i] = (unsigned int)malloc(64 * 1024 * sizeof(char));
		if ((char *)addrs[i] == NULL) {
			printf("cannot allocate");
		}
		get_proc();
	}

	for (int i = 0; i < size; ++i) {
		free((char *)addrs[i]);
	}
	return 0;
}
