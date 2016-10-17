#include <stdio.h>
int foo(int f)
{
  printf("%d\n", f);
}

int main()
{
  printf("%p\n", &foo);
}
