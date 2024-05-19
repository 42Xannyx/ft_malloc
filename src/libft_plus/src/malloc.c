#include <stdio.h>
#include <sys/mman.h>

#include "alloc.h"

void *ft_malloc(size_t size) {
  t_block block;

  const void *allocated_value =
      mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_ANONYMOUS, -1, 0);

  if (allocated_value == MAP_FAILED) {
    perror("mmap():");
    return NULL;
  }

  return (void *)allocated_value;
}
