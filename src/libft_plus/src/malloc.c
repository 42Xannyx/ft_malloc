#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"

/*
 * man malloc: To avoid corruption in multithreaded applications, mutexes
 * are used internally to protect the memory-management data structures
 * employed by these functions.used internally to protect the
 * memory-management data structures employed by these functions.
 */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static t_heap *heap = NULL;

void *ft_malloc(size_t size) {
  if (size <= 0) {
    return NULL;
  }

  pthread_mutex_lock(&mutex);
  const size_t aligned_size = align(size);

  t_block *block = NULL;
  if (!heap) {
    block = extend_heap(&heap, aligned_size);
  } else {
    block = extend_blocks(&heap, aligned_size);
  }

#ifdef DEBUG
  /*print_heap(heap);*/
#endif

  pthread_mutex_unlock(&mutex);
  return (void *)(block + 1);
}
