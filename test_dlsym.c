#include <dlfcn.h>
#include <stdio.h>

int main() {
  void *lib = dlopen("librevtracerwrapper.so", RTLD_NOW);
  if (!lib) {
    printf("lib not found\n");
    return -1;
  }
  void *func = dlsym(lib, "CallAllocateMemoryHandler");

  if (!func) {
    printf("func not found\n");
    return -1;
  }

  printf("success\n");
  return 0;
}
