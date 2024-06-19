#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "debug.h"

t_block *initiate_block(t_heap *heap, size_t size) {
  t_block *first_block = NULL;
  t_block *prev_block = NULL;
  int64_t remaining = size;

  uintptr_t current_position =
      (uintptr_t)heap + sizeof(t_heap); // skip metadata

  while (remaining > 0) {
    t_block *block = (t_block *)current_position;
    size_t block_size = determine_block_size(remaining);

    block->size = block_size;
    block->inuse = true;
    block->next = NULL;
    block->prev = prev_block;

    if (prev_block) {
      prev_block->next = block;
    } else {
      first_block = block;
    }

    prev_block = block;
    remaining = remaining - block_size;
    current_position = current_position + sizeof(t_block) + block_size;
  }

  return first_block;
}

void initialize_heap(t_heap *heap) {
  // memset(heap, 0, sizeof(t_heap));

  (void)heap;
  // heap->free_size = 0;
  // printf("Size: %lu\n", heap->block_count);
  // printf("Size: %lu\n", heap->total_size);
  // printf("Size: %lu\n", heap->free_size);
}
