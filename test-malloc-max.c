#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char **argv) {
  void *m = mmap(0, 0x80 + 4096, 0x3, 0x21, 0, 0);
  if (m == NULL)
    printf("Malloc failed\n");

  printf("Received address is %p\n", m);
  memcpy(m, argv[0], strlen(argv[0]));
  return 0;
}
