#include <stdio.h>

//static inline void native_store_gdt(struct desc_ptr *dtr)
//{
//  asm volatile("sgdt %0":"=m" (*dtr));
//}

int main() {
  unsigned short gdtr[3];

  // initialize device-driver's global variables
  asm(" sgdt %0 " : "=m" (gdtr) );
  static long gdtsize;
  static char *gdtbase;

  gdtsize = 1 + gdtr[0];
  gdtbase = (char *)(*(unsigned long *)(gdtr+1));

  printf("%ld %p\n", gdtsize, gdtbase);
  for (int i = 0; i < gdtsize; i++)
    printf("%x ", gdtbase[i]);
}
