#include <stdio.h>


#define GET_RETURN_ADDR() ({ int addr; asm volatile("mov 4(%%ebp), %0" : "=r" (addr) : :); addr; })


int main() {
  //int p, addr_p;
  //__asm__ ("mov 4(%%ebp), %0" : : "r" (p));
  //__asm__ ("lea 4(%%ebp), %0" : : "r" (addr_p));
  //printf("ret addr is %p %p\n", p, addr_p);

  int a = GET_RETURN_ADDR();
  printf("ret is %p\n", a);
}
