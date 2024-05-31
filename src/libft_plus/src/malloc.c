#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "alloc.h"

void *ft_malloc(size_t size) {
  t_block block;
  int32_t heap_size = getpagesize();

  const void *heap = (t_heap *)mmap(NULL, heap_size, PROT_WRITE | PROT_READ,
                                    MAP_PRIVATE | MAP_ANON, -1, 0);

  if (heap == MAP_FAILED) {
    perror("mmap()");
    return NULL;
  }

  printf("%p\n", heap);

  return (void *)heap;
}
