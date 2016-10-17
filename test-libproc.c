#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#include <time.h>

#include "os-linux.h"

long get_prss() {
  struct rusage rusage;
  getrusage( RUSAGE_SELF, &rusage );

  return (size_t)(rusage.ru_maxrss * 1024L);
}

ssize_t getfilesize(char *filename) {
  struct stat st;

  if (stat(filename, &st) == 0)
    return st.st_size;
}

int main(int argc, char **argv) {
  ssize_t len = getfilesize(argv[0]);
  printf("Size is %zu\n", len);
  printf("resident mem before malloc %ld %ld\n", get_prss(),
      get_rss());
  char *mem = (char*) malloc (len * sizeof(char));

  for (int i = 0; i < len; i += 100)
    mem[i] = 'A';

  printf("resident mem after malloc %ld %ld\n", get_prss(),
      get_rss());
  int fd = open(argv[0], O_RDONLY);
  void *map = mmap(0, len, PROT_READ, MAP_SHARED, fd, 0);
  int fd_copy = open("copy_src.c", O_RDWR | O_CREAT | O_TRUNC, 0644);
  ftruncate(fd_copy, len);
  void *map_copy = mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED,
      fd_copy, 0);

  printf("resident mem before mmap %ld %ld\n", get_prss(), get_rss());

  memcpy(map_copy, map, len);
  sleep(2);
  printf("resident mem after mmap %ld %ld\n", get_prss(), get_rss());

  munmap(map, len);
  sleep(2);
  printf("resident mem after munmap %ld %ld\n", get_prss(), get_rss());
  munmap(map_copy, len);
  sleep(2);
  printf("resident mem after munmap %ld %ld\n", get_prss(), get_rss());
  free(mem);
  sleep(2);
  printf("resident mem after free %ld %ld\n", get_prss(), get_rss());

  struct map_iterator mi;
  struct map_prot mp;
  unsigned long hi;
  unsigned long segbase, mapoff;

  if (maps_init (&mi, getpid()) < 0)
        return -1;

  long sum = 0;
   while (maps_next (&mi, &segbase, &hi, &mapoff, &mp)) {
     if (strstr(mi.path, "heap") != NULL) {
       sum += hi - segbase;
       break;
     }
  //   printf("%s %p %p %p %d %d\n", mi.path, segbase, hi, mapoff, mp.prot, mp.flags);
   }

   printf("Sum of rss is %ld\n", sum);

  close(fd);
  close(fd_copy);
  return 0;
}
