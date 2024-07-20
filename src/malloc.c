#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

bool create_large_heap(const size_t n) {
  if (n > (size_t)SMALL_HEAP_ALLOCATION_SIZE) {
    return true;
  }

  return false;
}

static t_block *allocate_memory(t_heap **heap, size_t aligned_size) {
  t_heap *tmp_heap = *heap;
  const t_amount amount_blocks = count_blocks(aligned_size);
  const size_t block_size = get_total_size(amount_blocks);
  const bool is_large = create_large_heap(aligned_size);

  while (tmp_heap && tmp_heap->next) {
    tmp_heap = tmp_heap->next;
  }

  // Heap is empty or is bigger than the acceptable heap size
  if (!g_heap || is_large == true) {
    DEBUG_PRINT_SIMPLE("Extending heap by being empty or a LARGE allocation");
    return extend_heap(&g_heap, amount_blocks, aligned_size);
  }

  // Blocks are not in use and can be reused
  /*size_t unused_blocks = amount_of_unused_blocks(*heap);*/
  /*if (unused_blocks >= amount_of_blocks) {*/
  /*  DEBUG_PRINT_SIMPLE("Reuse blocks");*/
  /*  return reuse_block(&tmp_heap, aligned_size);*/
  /*}*/

  // There is no more free space to allocate new blocks
  if ((int64_t)block_size > tmp_heap->free_size) {
    DEBUG_PRINT_SIMPLE("Call mmap()");
    return extend_heap(heap, amount_blocks, aligned_size);
  }

  // Add new blocks to the heap
  DEBUG_PRINT_SIMPLE("Add blocks");
  return extend_blocks(&tmp_heap, aligned_size);
}

void *ft_malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  print_sizes();

  pthread_mutex_lock(&g_mutex);

  t_block *block;
  const size_t aligned_size = align(size);

  block = allocate_memory(&g_heap, aligned_size);

  if (!block) {
    pthread_mutex_unlock(&g_mutex);
    return NULL;
  }

  pthread_mutex_unlock(&g_mutex);
  return (void *)((char *)block + sizeof(t_block));
}
