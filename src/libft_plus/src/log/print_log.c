#include <stdio.h>

#include "alloc.h"

#ifdef DEBUG
void print_sizes() {
  printf("\033[1;34mTINY_HEAP_ALLOCATION_SIZE\033[0m: %d bytes\n",
         TINY_HEAP_ALLOCATION_SIZE);
  printf("\033[1;34mTINY_BLOCK_SIZE\033[0m: %d bytes\n", TINY_BLOCK_SIZE);
  printf("\033[1;32mSMALL_HEAP_ALLOCATION_SIZE\033[0m: %d bytes\n",
         SMALL_HEAP_ALLOCATION_SIZE);
  printf("\033[1;32mSMALL_BLOCK_SIZE\033[0m: %d bytes\n", SMALL_BLOCK_SIZE);
}

void print_block(t_block *block) {
  printf("\033[1;33mBlock Address\033[0m: %p\n", (void *)block);
  printf("  \033[1;36mSize\033[0m: %zu bytes\n", block->size);
  printf("  \033[1;36mIn Use\033[0m: %s\n", block->inuse ? "Yes" : "No");
  printf("  \033[1;36mNext Block\033[0m: %p\n", (void *)block->next);
  printf("  \033[1;36mPrevious Block\033[0m: %p\n", (void *)block->prev);
}

void print_heap(t_heap *heap) {
  printf("\033[1;35mHeap Address\033[0m: %p\n", (void *)heap);
  printf("  \033[1;36mTotal Size\033[0m: %zu bytes\n", heap->total_size);
  printf("  \033[1;36mFree Size\033[0m: %zu bytes\n", heap->free_size);
  printf("  \033[1;36mBlock Count\033[0m: %zu\n", heap->block_count);
  printf("  \033[1;36mNext Heap\033[0m: %p\n", (void *)heap->next);
  printf("  \033[1;36mPrevious Heap\033[0m: %p\n", (void *)heap->prev);

  printf("  \033[1;36mBlocks:\033[0m\n");
  t_block *current_block = heap->blocks;
  while (current_block != NULL) {
    print_block(current_block);
    current_block = current_block->next;
  }
}
#endif // DEBUG
