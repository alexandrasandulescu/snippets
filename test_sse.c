#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LEN 1024

struct libc_ifunc_impl
{
	/* The name of function to be tested.  */
	const char *name;
	/* The address of function to be tested.  */
	void (*fn) (void);
	/* True if this implementation is usable on this machine.  */
	bool usable;
};

typedef size_t (*__libc_ifunc_impl_list_handle)(const char *name,
                       struct libc_ifunc_impl *array,
                       size_t max);
__libc_ifunc_impl_list_handle __libc_ifunc_impl_list;

extern void patch__rtld_global_ro();

int func(char *src) {
	char *str = (char*)malloc(MAX_LEN * sizeof(char));

	strncpy(str, src, strlen(src));

	if (strncmp(src, str, strlen(src)) == 0) {
		printf("Match\n");
	}
	return 0;
}

int main(int argc, char **argv) {
	struct libc_ifunc_impl func_list[32];

	patch__rtld_global_ro();

	void *mHandle = dlopen("libc.so.6", RTLD_NOW);

	__libc_ifunc_impl_list = (__libc_ifunc_impl_list_handle)dlsym(mHandle, "__libc_ifunc_impl_list");
	(__libc_ifunc_impl_list)("strlen", func_list, (sizeof(func_list) / sizeof(func_list[0])));

	for (int i = 0; i < (sizeof(func_list) / sizeof(func_list[0])); i++) {
		if (!func_list[i].fn || !func_list[i].name)
			break;
		printf("%p -> %s\n", func_list[i].fn, func_list[i].name);
	}

	return 0;
}
