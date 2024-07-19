#include <bits/posix1_lim.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "debug.h"

t_block *add_block(t_heap **heap, size_t size) {
  static uint64_t id_counter;
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
    current_position += sizeof(t_block) + tmp_block->size;
  } else {
    current_position =
        (uintptr_t)(*heap) + sizeof(t_heap); // Skip heap metadata
  }

  while (remaining > 0) {
    t_block *block = (t_block *)current_position;
    size_t total_block_size = determine_block_size(remaining);
    size_t usable_size = total_block_size - sizeof(t_block);

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
