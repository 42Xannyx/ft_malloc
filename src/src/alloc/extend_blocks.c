#include <stdio.h>
#include <sys/mman.h>

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

t_block *extend_blocks(t_heap **heap, const size_t size) {
  // | Metadata + requested allocation |
  const size_t amount_of_block_size = determine_total_block_size(size);
  const size_t amount_of_blocks = determine_amount_blocks(size);

  (*heap)->free_size = (*heap)->free_size - amount_of_block_size -
                       (sizeof(t_block) * amount_of_blocks);
  (*heap)->block_count = (*heap)->block_count + amount_of_blocks;
  t_block *block = add_block(heap, size);

  t_block *tmp_block = (*heap)->last_block;

  while (tmp_block->next) {
    tmp_block = tmp_block->next;
  }

  (*heap)->last_block = tmp_block;

#ifdef DEBUG
  /*printf("\n-------------------\n");*/
  /*printf("\nNew blocks\n");*/

  print_heap(*heap, false);
#endif

  return block;
}
