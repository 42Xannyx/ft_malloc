#include <stdio.h>
#include <sys/mman.h>

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

#define PROT (PROT_READ | PROT_WRITE)
#define MAP (MAP_ANONYMOUS | MAP_PRIVATE)

t_block *extend_heap(t_heap **heap, const size_t size) {
  // | Metadata + requested allocation |
  const size_t amount_of_blocks = determine_amount_blocks(size);
  const size_t amount_of_block_size = determine_total_block_size(size);
  const size_t total_size =
      determine_heap_size(amount_of_block_size, amount_of_blocks);

#ifdef DEBUG
  printf("Calling mmap with total_size %zu\n", total_size);
  fflush(stdout);
#endif
  t_heap *tmp_heap = (t_heap *)mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                                    MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (tmp_heap == MAP_FAILED) {
    perror("mmap()");
    return NULL;
  }
#ifdef DEBUG
  printf("mmap returned %p\n", (void *)tmp_heap);
  fflush(stdout);
#endif

  tmp_heap->total_size = total_size;
  // I remove an addition 128 bytes, because I need to make space of
  // sizeof(t_heap)
  tmp_heap->free_size = total_size - amount_of_block_size - 128 -
                        (sizeof(t_block) * amount_of_blocks);
  tmp_heap->block_count = amount_of_blocks;

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

  return block;
}
