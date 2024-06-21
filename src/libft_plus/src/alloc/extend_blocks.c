#include <stdio.h>
#include <sys/mman.h>

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

t_block *extend_blocks(t_heap **heap, const size_t size) {
  // | Metadata + requested allocation |
  const size_t amount_of_block_size = determine_total_block_size(size);

  (*heap)->free_size = (*heap)->free_size - amount_of_block_size;
  (*heap)->block_count = (*heap)->block_count + determine_amount_blocks(size);
  t_block *block = add_block(heap, size);

#ifdef DEBUG
  printf("New blocks\n");

  t_block *tmp_block = block;
  while (tmp_block->next) {
    print_block(tmp_block);
    tmp_block = tmp_block->next;
  }
  print_block(tmp_block);
#endif

  return block;
}
