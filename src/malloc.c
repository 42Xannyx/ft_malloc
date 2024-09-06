#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"
#include "shared.h"

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

static bool create_large_heap(const size_t n) {
  return n > (size_t)SMALL_HEAP_ALLOCATION_SIZE;
}

static t_block *allocate_memory(t_heap **heap, const size_t aligned_size) {
  t_heap *tmp_heap = *heap;
  bool is_large = create_large_heap(aligned_size);

  while (tmp_heap && tmp_heap->next) {
    tmp_heap = tmp_heap->next;
  }

  // Heap is empty or is bigger than the acceptable heap size
  if (!*heap || is_large == true) {
    DEBUG_PRINT_SIMPLE("Extending heap by being empty or a LARGE allocation");
    return extend_heap(&g_heap, is_large, aligned_size);
  }

  // Blocks are not in use and can be reused
  bool enough_space = find_enough_unused_space(*heap, aligned_size);
  if (enough_space == true) {
    DEBUG_PRINT_SIMPLE("Reuse blocks");
    return reuse_block(&tmp_heap, aligned_size);
  }

  // There is no more free space to allocate new blocks
  const size_t block_size = aligned_size + SIZEOF_BLOCK;
  if ((int64_t)block_size > tmp_heap->free_size) {
    DEBUG_PRINT_SIMPLE("Call mmap()");
    return extend_heap(heap, false, aligned_size);
  }

  // Add new blocks to the heap
  DEBUG_PRINT_SIMPLE("Add blocks");
  t_block *blocks = extend_blocks(&tmp_heap, aligned_size);

  *heap = tmp_heap;
  return blocks;
}

void *ft_malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  pthread_mutex_lock(&g_mutex);

  t_block *block;
  const size_t aligned_size = align(size);

  block = allocate_memory(&g_heap, aligned_size);

  if (!block) {
    pthread_mutex_unlock(&g_mutex);
    return NULL;
  }

  block->magic_start = BLOCK_MAGIC;
  t_block *tmp = block;

  while (tmp && tmp->next) {
    tmp = tmp->next;
  }

  uint32_t *magic_end = (uint32_t *)((char *)tmp + SIZEOF_BLOCK + tmp->size);
  *magic_end = BLOCK_MAGIC;

  pthread_mutex_unlock(&g_mutex);
  return (void *)((char *)block + sizeof(t_block));
}
