#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#define MAX_PATH_NAME 100

bool FindModuleInDir(const char *moduleName, char *dirname, char *path) {
	memset(path, 0, MAX_PATH_NAME);

	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if (!strcmp(moduleName, dir->d_name)) {
				strcpy(path, dirname);
				path[strlen(path)] = '/';
				strcat(path, moduleName);

				closedir(d);
				return true;
			}
			//printf("%s\n", dir->d_name);
		}

		closedir(d);
	}

	return false;
}

int main() {
  char *env = getenv("LD_LIBRARY_PATH");
  printf("%s\n", env);

	// iterate thourgh it
	const char *it = env;
	const char *start = it;
	while (1) {
		if ((*it == ':') || (*it == '\0')) {
			//process from start to it - 1
			ssize_t len = it - 1 - start + 1;
			if (!len)
				continue;
			char dirname[MAX_PATH_NAME];
			memset(dirname, 0, MAX_PATH_NAME);
			strncpy(dirname, start, len);
      char path[MAX_PATH_NAME];
			if (FindModuleInDir("test-env.c", dirname, path)) {
        printf("found path : %s\n", path);
				return true;
			}
			printf("%s\n", dirname);
			if (!*it)
				break;
			start = it + 1;
		}
		it++;
	}
  return 0;
}
