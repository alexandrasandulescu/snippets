#include <string.h>
#include <stdio.h>
int main(int argc, char **argv) {

  unsigned int a = strlen(argv[0]);
  unsigned int b = strlen(argv[1]);
  unsigned int ret = 0x0;

  for (int i = 0; i < 32; i++) {
    ret |= (((a >> i) & 0x1) | ((b >> i) & 0x1)) << i;
  }

  printf("%u %u %u\n", a, b, ret);
}
