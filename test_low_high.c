#include <stdio.h>
#include <stdint.h>

int main() {
  uint32_t nr = 0xAABBCCDD;
  int byte_len = (1 << 3);

  uint16_t nr_high = nr >> (2 * byte_len);
  uint16_t nr_low = (uint16_t)(nr & 0x0000FFFF);

  printf("low: %04X high: %04X\n", nr_low, nr_high);
  return 0;
}
