#include <stdio.h>
#include <sys/mman.h>

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

#define PROT (PROT_READ | PROT_WRITE)
#define MAP (MAP_ANONYMOUS | MAP_PRIVATE)

t_block *extend_heap(t_heap **heap, const size_t size) {
  // | Metadata + requested allocation |
  t_block *block = NULL;
  const size_t total_size = sizeof(t_heap) + determine_heap_size(size);
  const size_t amount_of_block_size = determine_total_block_size(size);

  t_heap *tmp_heap = (t_heap *)mmap(NULL, total_size, PROT, MAP, -1, 0);
  if (tmp_heap == MAP_FAILED) {
    perror("mmap()");
    return NULL;
  }

  tmp_heap->total_size = total_size;
  tmp_heap->free_size = total_size - amount_of_block_size - sizeof(t_heap);
  tmp_heap->block_count = determine_amount_blocks(size);

  if (!*heap) {
    tmp_heap->next = NULL;
    tmp_heap->prev = NULL;
    (*heap) = tmp_heap;

    block = add_block(heap, size);
  } else {
    block = add_block(&tmp_heap, size);

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
    print_heap(*heap);
    print_heap((*heap)->next);
  } else {
    print_heap(*heap);
  }
#endif

  return block;
}
