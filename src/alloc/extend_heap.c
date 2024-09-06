#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

#define PROT (PROT_READ | PROT_WRITE)
#define MAP (MAP_ANONYMOUS | MAP_PRIVATE)

t_block *extend_heap(t_heap **heap, bool is_large, const size_t size) {
  // | Metadata + requested allocation |
  const size_t total_size = get_total_size(is_large, size);

  DEBUG_PRINT("Calling mmap with total_size %zu\n", total_size);
  t_heap *tmp_heap = (t_heap *)mmap(NULL, total_size, PROT, MAP, -1, 0);
  if (tmp_heap == MAP_FAILED) {
    perror("mmap()");
    return NULL;
  }

  tmp_heap->total_size = total_size;
  tmp_heap->free_size = total_size - size - SIZEOF_HEAP - SIZEOF_BLOCK;
  tmp_heap->block_count++;

  t_block *block = NULL;
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

  // Add blocks to the new heap
  t_block *block = add_block(&tmp_heap, size);
  if (!block) {
    // Handle allocation failure
    munmap(tmp_heap, total_size);
    return NULL;
  }

  // Calculate actual free size after block allocation
  tmp_heap->free_size =
      total_size - (tmp_heap->block_count * SIZEOF_BLOCK) - SIZEOF_HEAP - size;

  // Set last_block directly from add_block result
  tmp_heap->last_block = block;
  while (tmp_heap->last_block->next) {
    tmp_heap->last_block = tmp_heap->last_block->next;
  }

#ifdef DEBUG
  print_heap(tmp_heap, false);
#endif

  return block;
}
