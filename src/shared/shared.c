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

  /*while (block) {*/
  /*  if (block->next == NULL) {*/
  /*    uint32_t *magic_end =*/
  /*        (uint32_t *)((char *)block + SIZEOF_BLOCK + block->size);*/
  /*    DEBUG_PRINT("Checking Magic End at %p: %u\n", (void *)magic_end,*/
  /*                *magic_end);*/
  /*    if (*magic_end != BLOCK_MAGIC) {*/
  /**/
  /*      fprintf(stderr, "Buffer overflow detected at end of block %p!\n",*/
  /*              (void *)block);*/
  /*      abort();*/
  /*    }*/
  /*    break;*/
  /*  }*/
  /**/
  /*  block = block->next;*/
  /*}*/

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
    DEBUG_PRINT("size: %zu\n", size);
    DEBUG_PRINT("size: %zu\n", block->size);
    if (block->inuse == false && block->size >= size - SIZEOF_BLOCK) {

      heap->unused_block = block;
      return true;
    }
    block = block->next;
  }

  return false;
}

/*t_amount amount_of_unused_space(t_heap *heap) {*/
/*  t_amount space = {0};*/
/*  t_block *block = heap->blocks;*/
/**/
/*  while (block) {*/
/*    if (block->inuse == false) {*/
/*      heap->unused_block = block;*/
/*      if (block->size == (size_t)TINY_BLOCK_SIZE - SIZEOF_BLOCK) {*/
/*        space.tiny = space.tiny + 1;*/
/*      } else {*/
/*        space.small = space.small + 1;*/
/*      }*/
/*      if (block->next && block->inuse == true) {*/
/*        return space;*/
/*      }*/
/*    }*/
/*    block = block->next;*/
/*  }*/
/**/
/*  return space;*/
/*}*/
