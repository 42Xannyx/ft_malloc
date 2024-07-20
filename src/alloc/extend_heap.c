#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

#define PROT (PROT_READ | PROT_WRITE)
#define MAP (MAP_ANONYMOUS | MAP_PRIVATE)

t_block *extend_heap(t_heap **heap, t_amount amount_blocks, const size_t size) {
  // | Metadata + requested allocation |
  const size_t block_count = get_amount_blocks(amount_blocks);
  const size_t amount_of_block_size = determine_total_block_size(size);
  const size_t total_size = get_total_size(amount_blocks);

  DEBUG_PRINT("Calling mmap with total_size %zu\n", total_size);

  t_heap *tmp_heap = (t_heap *)mmap(NULL, total_size, PROT, MAP, -1, 0);
  if (tmp_heap == MAP_FAILED) {
    perror("mmap()");
    return NULL;
  }

  DEBUG_PRINT("mmap returned %p\n", (void *)tmp_heap);

  tmp_heap->total_size = total_size;
  tmp_heap->free_size = total_size - amount_of_block_size - SIZEOF_HEAP -
                        (SIZEOF_BLOCK * block_count);
  tmp_heap->block_count = block_count;

  t_block *block = NULL;
  if (!*heap) {
    tmp_heap->next = NULL;
    tmp_heap->prev = NULL;
    (*heap) = tmp_heap;

    block = add_block(heap, amount_of_block_size);
  } else {
    block = add_block(&tmp_heap, amount_of_block_size);

    tmp_heap->prev = *heap;
    (*heap)->next = tmp_heap;
  }

  t_block *tmp_block = block;

  while (tmp_block->next) {
    tmp_block = tmp_block->next;
  }

  tmp_heap = *heap;

  while (tmp_heap->next) {
    tmp_heap = tmp_heap->next;
  }

  tmp_heap->last_block = tmp_block;

  *heap = tmp_heap;

#ifdef DEBUG
  if ((*heap)->next) {
    print_heap(*heap, false);
    print_heap((*heap)->next, false);
  } else {
    print_heap(*heap, false);
  }
#endif

  *(size_t *)((char *)(block + 1) + size - sizeof(size_t)) = BLOCK_MAGIC;

  return block;
}
