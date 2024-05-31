#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "alloc.h"

void *ft_malloc(size_t size) {
  // t_block block;

  const size_t aligned_size = align(size);
  const t_heap *heap =
      (t_heap *)mmap(NULL, aligned_size, PROT_WRITE | PROT_READ,
                     MAP_PRIVATE | MAP_ANON, -1, 0);

  if (heap == MAP_FAILED) {
    perror("mmap()");
    return NULL;
  }

  word_t myWord = (word_t)0x12345678;
  printf("Value of myWord: %lx\n", sizeof(myWord));
  printf("Current address: %p\n", heap);
  printf("Total size: %ld\n", heap->total_size);
  printf("Block count: %ld\n", heap->block_count);
  printf("Free size: %ld\n", heap->free_size);

  return (void *)heap;
}
