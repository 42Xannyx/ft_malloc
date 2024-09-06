#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "debug.h"

static uintptr_t get_current_position(t_heap *heap, t_block *tmp_block) {
  uintptr_t current_position = (uintptr_t)tmp_block;

  if (tmp_block) {
    current_position += SIZEOF_BLOCK + tmp_block->size;
    return current_position;
  }

  return (uintptr_t)(heap) + sizeof(t_heap); // Skip heap metadata
}

t_block *add_block(t_heap **heap, size_t size) {
  size_t usable_size = size - SIZEOF_BLOCK;
  t_block *tmp_block = (*heap)->last_block;

  if (tmp_block) {
    while (tmp_block->next) {
      tmp_block = tmp_block->next;
    }
  }

  uintptr_t current_position = get_current_position(*heap, tmp_block);
  t_block *block = (t_block *)current_position;

  block->size = usable_size;
  block->inuse = true;
  block->next = NULL;
  block->prev = tmp_block;

  if (!(*heap)->blocks) {
    (*heap)->blocks = block;
  }

  return block;
}

t_block *reuse_block(t_heap **heap, size_t size) {
  int64_t remaining = size;
  t_block *block = (*heap)->unused_block;
  t_block *first_new_block = (*heap)->unused_block;

  while (remaining > 0) {
    if (!block || block->inuse == true) {
      // TODO: Error message to not have enough space
      return NULL;
    }

    block->inuse = true;

    remaining -= block->size;
    block = block->next;
  }

  (*heap)->unused_block = NULL;

#ifdef DEBUG
  print_heap(*heap, false);
#endif

  return first_new_block;
}
