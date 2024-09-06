#include "alloc.h"
#include "libft_plus.h"
#include "shared.h"

#include <pthread.h>
#include <stdlib.h>

void *ft_realloc(void *ptr, size_t size) {
  pthread_mutex_lock(&g_mutex);

  if (!ptr) {
    write(STDERR_FILENO, "ptr empty\n", 11);

    pthread_mutex_unlock(&g_mutex);
    return NULL;
  }

  if (!g_heap) {
    write(STDERR_FILENO, "heap is empty\n", 15);

    pthread_mutex_unlock(&g_mutex);
    return NULL;
  }

  size_t aligned_size = align(size);
  t_block *block = (t_block *)((char *)ptr - sizeof(t_block));
  t_heap *heap = find_heap_for_block(block);

  if ((size_t)heap->free_size >= aligned_size) {
    block->size = aligned_size;

    uint32_t *magic_end =
        (uint32_t *)((char *)block + SIZEOF_BLOCK + block->size);
    *magic_end = BLOCK_MAGIC;

    pthread_mutex_unlock(&g_mutex);
    return ptr;
  }

  t_block *heap_block = extend_heap(&heap, true, aligned_size);
  (void)heap_block;

  pthread_mutex_unlock(&g_mutex);
  return ptr;
}
