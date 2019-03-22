#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>


#define BYTE_LEN 8
#define MAX_UINT_8 ((1 << 8) - 1)

uint8_t get_rand_8() {
  uint8_t nr = rand() %  MAX_UINT_8;
  printf("%02X ", nr);
  return nr;
}

uint16_t get_rand_16() {
  return (get_rand_8() << BYTE_LEN) | get_rand_8();
}

uint32_t get_rand_32() {
  return (get_rand_16() << (2 * BYTE_LEN)) | get_rand_16();
}

int main() {
  srand(time(NULL));
  uint32_t nr = get_rand_32();
  printf("=> %08X\n", nr);
  return 0;
}
