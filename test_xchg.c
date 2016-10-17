
char a[100];
int main()
{
  int p = a;
  asm volatile("xchgl %0,%%esp" : : "m" (p));
  asm volatile("xchgl %0,%%esp" : : "m" (p));
  return 0;
}
