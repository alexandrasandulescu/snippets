#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/resource.h>
#include <string.h>

#include <errno.h>

void pretty_print(int number) {
  printf("0x%08X", number);
}

int main() {


  struct rlimit rlimit;
  int res = getrlimit(RLIMIT_DATA, &rlimit);

  if (res) {
    printf("Failed to get rlimit for data segment\n");
    exit(1);
  }

  printf("Data limit soft: 0x%lX hard: 0x%lX\n", rlimit.rlim_cur, rlimit.rlim_max);
  FILE *f = fopen("/proc/self/status", "r");
  char line[256];
  while (!feof(f)) {
    fgets(line, sizeof(line), f);
    if (strncmp(line, "VmSize", 6) == 0) {
      printf("%s", line);
      break;
    }
  }
  fclose(f);


  size_t success_max = 0;
  size_t fail_min = 0xFFFFFFFF;

  size_t low = 0;
  size_t high = 0xFFFFFFFF; // 4GB
  while(low <= high) {
    size_t msize = (size_t)(((uint64_t)low + (uint64_t)high) >> 1);
    void *p = malloc((size_t)(msize));
    if (p) {
      if (msize == 0)
        break;
      //printf("Worked for: 0x%08X; low: 0x%08X high: 0x%08X\n", msize, low, high);
      if (success_max < msize) success_max = msize;
      low = msize + 1;
      high = high;
      printf("Got address %p for size: 0x%08X\n", p, msize);
      free(p);
    } else {
      //printf("Didn't work for: ");
      //pretty_print(msize);
      //printf("\n");
      if (fail_min > msize) fail_min = msize;
      if (errno == ENOMEM) {
        printf("Failed with enomem\n");
      }
      low = low;
      high = msize - 1;
    }
  }

  printf("Max alloc size: 0x%08X; max failed: 0x%08X\n", success_max, fail_min);

  return 0;
}
