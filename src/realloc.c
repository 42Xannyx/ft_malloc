#include "alloc.h"
#include "libft_plus.h"
#include "shared.h"

#include <errno.h>
#include <sys/resource.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void *ft_realloc(void *ptr, size_t size) {
  pthread_mutex_lock(&g_mutex);

  if (!ptr) {
    pthread_mutex_unlock(&g_mutex);
    void *new_ptr = ft_malloc(size);
    return new_ptr;
  }

  struct rlimit as_lim, data_lim;

  // Check size of heap & size of virtual space
  if (getrlimit(RLIMIT_AS, &as_lim) == -1 || getrlimit(RLIMIT_DATA, &data_lim) == -1) {
    pthread_mutex_unlock(&g_mutex);
    errno = ENOMEM;
    return NULL;
  }

  if (size == 0) {
    pthread_mutex_unlock(&g_mutex);
    ft_free(ptr);
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

  if (block->size >= aligned_size) {
    pthread_mutex_unlock(&g_mutex);
    return ptr;
  }

  if ((size_t)heap->free_size >= (aligned_size - block->size)) {
    size_t old_size = block->size;

    block->size = aligned_size;
    heap->free_size -= (aligned_size - old_size);

    uint32_t *magic_end =
        (uint32_t *)((char *)block + SIZEOF_BLOCK + block->size);
    *magic_end = BLOCK_MAGIC;

    pthread_mutex_unlock(&g_mutex);
    return ptr;
  }

  void *new_ptr = ft_malloc(size);
  if (!new_ptr) {
    pthread_mutex_unlock(&g_mutex);
    return NULL;
  }

  memcpy(new_ptr, ptr, block->size);

  ft_free(ptr);

  pthread_mutex_unlock(&g_mutex);
  return new_ptr;
}
