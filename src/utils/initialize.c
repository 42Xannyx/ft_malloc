#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "debug.h"

static uint64_t id_counter;

t_block *add_block(t_heap **heap, size_t size) {
  int64_t remaining = size;
  t_block *tmp_block = (*heap)->last_block;
  t_block *first_new_block = NULL;

  if (tmp_block) {
    while (tmp_block->next) {
      tmp_block = tmp_block->next;
    }
  }

  uintptr_t current_position = (uintptr_t)tmp_block;
  if (tmp_block) {
    current_position += SIZEOF_BLOCK + tmp_block->size;
  } else {
    current_position =
        (uintptr_t)(*heap) + sizeof(t_heap); // Skip heap metadata
  }

  while (remaining > 0) {
    t_block *block = (t_block *)current_position + SIZEOF_BLOCK;
    size_t total_block_size = determine_block_size(remaining);
    size_t usable_size = total_block_size - SIZEOF_BLOCK;

    block->_id = id_counter;
    block->size = usable_size;
    block->inuse = true;
    block->next = NULL;
    block->prev = tmp_block;

    if (tmp_block) {
      tmp_block->next = block;
    } else {
      (*heap)->blocks = block;
    }

    if (first_new_block == NULL) {
      first_new_block = block;
    }

    tmp_block = block;
    remaining -= usable_size;
    current_position += total_block_size;
  }

  id_counter = id_counter + 1;
  return first_new_block;
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

    block->_id = id_counter;
    block->inuse = true;

    remaining -= block->size;
    block = block->next;
  }

  id_counter = id_counter + 1;
  (*heap)->unused_block = NULL;

#ifdef DEBUG
  print_heap(*heap, false);
#endif

  return first_new_block;
}
