#include <stdio.h>
#include <stdint.h>

struct A {
  struct AA { uint16_t _a; } a;
  union Y {} y;
} a;

struct B {
  union Z {} z;
};

union C {} c;

int main() {
  struct A aa;
  *((uint8_t *)&aa.y) = 0x00;
  printf("(%d %d) %d %d\n", sizeof(a), sizeof(a.y), sizeof(struct B), sizeof(c));
  return 0;
}
