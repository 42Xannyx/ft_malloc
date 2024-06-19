#include <stdio.h>
#include <sys/mman.h>

#include "alloc.h"

#define PROT (PROT_READ | PROT_WRITE)
#define MAP (MAP_ANONYMOUS | MAP_PRIVATE)

t_block *initiate_block(t_heap *heap, size_t size) {
  t_block *block = (t_block *)(heap + 1);
  block->size = size;
  block->inuse = true;
  block->next = NULL;

  return block;
}

t_block *extend_heap(t_heap *heap, const size_t size) {
  // | Metadata + requested allocation |
  const size_t total_size = sizeof(t_block) + size;

  t_heap *tmp_heap = (t_heap *)mmap(NULL, total_size, PROT, MAP, -1, 0);
  if (tmp_heap == MAP_FAILED) {
    perror("mmap()");
    return NULL;
  }

  tmp_heap->total_size = total_size;
  tmp_heap->free_size = size;
  tmp_heap->block_count = 1;
  tmp_heap->next = heap;
  heap = tmp_heap;

  t_block *block = initiate_block(tmp_heap, size);
  heap->blocks = block;

  return block;
}
