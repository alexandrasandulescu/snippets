#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef int (*_printf_handler)(const char *format, ...);
_printf_handler _printf;

int main() {
	void *handle = dlopen("/lib/i386-linux-gnu/libc.so.6", RTLD_LAZY);
	assert(handle != NULL);

	void *sym = dlsym(handle, "printf");
	_printf = (_printf_handler)sym;

	_printf("found symbol printf @@%08lx\n", (unsigned long)sym);
	sym = dlsym(handle, "__gconv_get_modules_db");

	_printf("found symbol printf @@%08lx\n", (unsigned long)sym);
	return 0;
}
