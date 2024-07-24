#include "alloc.h"
#include "libft_plus.h"

#include <stdio.h>
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

bool find_enough_unused_space(t_heap *heap, t_amount amount) {
  t_amount space = {0};
  t_block *block = heap->blocks;

  while (block) {
    if (block->inuse == false) {
      if (block->size == (size_t)TINY_BLOCK_SIZE - SIZEOF_BLOCK) {
        space.tiny = space.tiny + 1;
      } else {
        space.small = space.small + 1;
      }

      if (((block->next && block->next->inuse == true) || !block->next) &&
          space.small >= amount.small && space.tiny >= amount.tiny) {
        while (block->prev && block->prev->inuse == false) {
          block = block->prev;
        }
        heap->unused_block = block;
        return true;
      } else {
        space.tiny = 0;
        space.small = 0;
      }
    }
    block = block->next;
  }

  return false;
}

t_amount amount_of_unused_space(t_heap *heap) {
  t_amount space = {0};
  t_block *block = heap->blocks;

  while (block) {
    if (block->inuse == false) {
      heap->unused_block = block;
      if (block->size == (size_t)TINY_BLOCK_SIZE - SIZEOF_BLOCK) {
        space.tiny = space.tiny + 1;
      } else {
        space.small = space.small + 1;
      }
      if (block->next && block->inuse == true) {
        return space;
      }
    }
    block = block->next;
  }

  return space;
}
