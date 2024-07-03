#ifndef LIBFT_PLUS_H
#define LIBFT_PLUS_H

#include <stdint.h>
#include <unistd.h>

#include "alloc.h"

//******** Allocator ********//

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *ft_realloc(void *ptr, size_t size);

//******** Functions ********//

__attribute__((warn_unused_result)) t_block *extend_heap(t_heap **heap,
                                                         size_t size);
__attribute__((warn_unused_result)) t_block *extend_blocks(t_heap **heap,
                                                           size_t size);
void initialize_heap(t_heap *heap);
t_block *add_block(t_heap **heap, size_t size);

//******** Inline Functions ********//

__attribute__((warn_unused_result)) static inline size_t
determine_amount_blocks(size_t n) {
  if (n > (size_t)SMALL_HEAP_ALLOCATION_SIZE) {
    return 1;
  }

  size_t amount = n / SMALL_BLOCK_SIZE;

  if (n % SMALL_BLOCK_SIZE > 0) {
    amount += 1;
  }

  return amount;
}

#endif
