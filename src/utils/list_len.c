#include "alloc.h"

size_t list_len(void *lst) {
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
