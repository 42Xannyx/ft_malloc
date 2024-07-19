#include "libft_plus.h"

#include <stdlib.h>

t_heap *g_heap = NULL;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void check_block_integrity(t_block *block) {
  size_t total_size = sizeof(t_block) + block->size;
  size_t *end_canary = (size_t *)((char *)block + total_size);
  if (*end_canary != BLOCK_MAGIC) {
    fprintf(stderr, "Buffer overflow detected in block %p\n", (void *)block);
    /*abort();*/
  }
}

bool blocks_inuse(t_heap *heap) {
  t_block *block = heap->blocks;

  while (block) {
    if (block->inuse == true) {
      return true;
    }
    block = block->next;
  }

  return false;
}

bool any_block_not_inuse(t_heap *heap) {
  t_block *block = heap->blocks;

  while (block) {
    if (block->inuse == false) {
      return true;
    }
    block = block->next;
  }

  return false;
}
