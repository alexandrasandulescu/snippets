#include <stdio.h>
int __attribute__((cdecl)) bar(int a, int b, int c) {
  printf("bar\n");
  return 0;
}

int __attribute__((stdcall)) foo(int a, int b, int c) {
  printf("foo\n");
  return 0;
}

int main() {
  foo(0, 1, 2);
  bar(0, 1, 2);
  return 0;
}
