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
    printf("First extend of heap\n");
    block = extend_heap(&heap, aligned_size);
  } else {
    t_heap *tmp_heap = heap;
    size_t block_size = determine_block_size(aligned_size);

    while (tmp_heap->next) {
      tmp_heap = tmp_heap->next;
    }

    if (block_size > tmp_heap->free_size) {
      printf("Call mmap()\n");
      block = extend_heap(&heap, aligned_size);
    } else {
      printf("Add blocks\n");
      block = extend_blocks(&tmp_heap, aligned_size);
      heap = tmp_heap;
    }
  }

#ifdef DEBUG
  /*print_heap(heap);*/
#endif

  pthread_mutex_unlock(&mutex);
  return (void *)(block + 1);
}
