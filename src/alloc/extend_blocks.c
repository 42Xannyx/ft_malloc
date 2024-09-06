#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

t_block *extend_blocks(t_heap **heap, const size_t size) {
  // | Metadata + requested allocation |
  (*heap)->free_size = (*heap)->free_size - size - SIZEOF_BLOCK;
  (*heap)->block_count = (*heap)->block_count + 1;
  t_block *block = add_block(heap, size);

  t_block *tmp_block = (*heap)->last_block;

  while (tmp_block->next) {
    tmp_block = tmp_block->next;
  }

  (*heap)->last_block = tmp_block;

#ifdef DEBUG
  print_heap(*heap, true);
#endif

  return block;
}
