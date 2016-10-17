#include <stdio.h>
#define LOCK_XCHG(target, value) asm volatile("xchgl %0, %1" : "+m"(*target) : "r"(value) : "memory")

int main(int argc, char **argv)
{
  if (!(argc & 2))
    asm volatile("int $0x3");

  int x = 4;
  int *p = &x;
  int value = 3;
  LOCK_XCHG(&x, 3);

  printf("%s %d\n", argv[0], x);
  return 0;

}
