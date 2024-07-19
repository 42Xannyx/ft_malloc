#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"
#include "shared.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

static t_block *allocate_memory(t_heap **heap, size_t aligned_size) {
  t_heap *tmp_heap = *heap;
  size_t block_size = determine_total_block_size(aligned_size);

  while (tmp_heap->next) {
    tmp_heap = tmp_heap->next;
  }

  if ((int64_t)block_size > tmp_heap->free_size) {
    DEBUG_PRINT_SIMPLE("Call mmap()");
    return extend_heap(heap, aligned_size);
  }

  if (any_block_not_inuse(*heap)) {
    DEBUG_PRINT_SIMPLE("Reuse blocks");
    return reuse_block(&tmp_heap, aligned_size);
  }

  DEBUG_PRINT_SIMPLE("Add blocks");
  return extend_blocks(&tmp_heap, aligned_size);
}

void *ft_malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  pthread_mutex_lock(&g_mutex);

  const size_t aligned_size = align(size);
  const size_t amount_of_blocks = determine_block_size(aligned_size);
  const size_t determined_heap_size =
      determine_heap_size(aligned_size, amount_of_blocks);

  t_block *block;
  if (!g_heap || (determined_heap_size > (size_t)SMALL_HEAP_ALLOCATION_SIZE)) {
    DEBUG_PRINT_SIMPLE("Extending heap");
    block = extend_heap(&g_heap, aligned_size);
  } else {
    block = allocate_memory(&g_heap, aligned_size);
  }

  if (!block) {
    pthread_mutex_unlock(&g_mutex);
    return NULL;
  }

  pthread_mutex_unlock(&g_mutex);
  return (void *)((char *)block + sizeof(t_block));
}
