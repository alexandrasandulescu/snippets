#include <stdlib.h>

int main() {
  int a[10];

  int *x = (int *)malloc(100);
  for (int i = 0; i < 10; ++i)
    a[i] = i ^ (i + i * i + i / 2);

  free(x);
  return 0;
}
