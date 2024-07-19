#include "alloc.h"
#include <stdio.h>

size_t block_list_len(void *lst) {
  if (!lst) {
    return 0;
  }

  size_t len = 1;
  t_block *block = (t_block *)lst;

  while (block->next) {
    block = block->next;
    len++;
  }

  return len;
}

size_t block_list_len_by_id(void *lst) {
  if (!lst) {
    return 0;
  }

  size_t len = 0;
  t_block *block = (t_block *)lst;
  ssize_t current_id = block->_id;

  while (block && current_id == block->_id) {
    block = block->next;
    len++;
  }

  return len;
}
