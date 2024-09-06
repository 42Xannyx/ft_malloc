#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

#include <stdio.h>
#include <stdlib.h>

t_heap *g_heap = NULL;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

bool check_buffer_overflow(t_block *block) {
  if (!block)
    return false;

  if (block->magic_start != BLOCK_MAGIC) {
    fprintf(stderr, "Buffer underflow detected at block %p!\n", (void *)block);
    abort();
  }

  while (block) {
    if (block->next == NULL) {
      uint32_t *magic_end =
          (uint32_t *)((char *)block + SIZEOF_BLOCK + block->size);
      DEBUG_PRINT("Checking Magic End at %p: %u\n", (void *)magic_end,
                  *magic_end);
      if (*magic_end != BLOCK_MAGIC) {

        fprintf(stderr, "Buffer overflow detected at end of block %p!\n",
                (void *)block);
        abort();
      }
      break;
    }

    block = block->next;
  }

  return false;
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

bool find_enough_unused_space(t_heap *heap, const size_t size) {
  t_block *block = heap->blocks;

  while (block) {
    if (block->inuse == false && block->size >= size) {

      heap->unused_block = block;
      return true;
    }
    block = block->next;
  }

  return false;
}

t_heap *find_heap_for_block(t_block *block) {
  t_heap *current_heap = g_heap;

  while (current_heap) {
    uintptr_t heap_start = (uintptr_t)current_heap;
    uintptr_t heap_end = heap_start + current_heap->total_size;

    if ((uintptr_t)block >= heap_start && (uintptr_t)block < heap_end) {
      return current_heap;
    }
    current_heap = current_heap->prev;
  }
  return NULL;
}
