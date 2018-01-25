#include <dlfcn.h>
#include <stdio.h>
int main() {
  void * p = dlopen("/home/alex/river/ParserPayload/libhttp-parser.so", RTLD_LAZY);
  printf("%p\n", p);
  return 0;
}
