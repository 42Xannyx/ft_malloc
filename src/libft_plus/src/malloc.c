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

  t_block *block = NULL;
  const size_t aligned_size = align(size);
  const size_t determined_heap_size = determine_heap_size(aligned_size);

  if (!heap || determined_heap_size > (size_t)SMALL_HEAP_ALLOCATION_SIZE) {
    printf("First extend of heap\n");
    block = extend_heap(&heap, aligned_size);
  } else {
    t_heap *tmp_heap = heap;
    size_t block_size = determine_block_size(aligned_size);

    while (tmp_heap->next) {
      tmp_heap = tmp_heap->next;
    }

    if (block_size > (size_t)tmp_heap->free_size) {
#ifdef DEBUG
      printf("Call mmap()\n");
#endif
      block = extend_heap(&heap, aligned_size);
    } else {
#ifdef DEBUG
      printf("Add blocks\n");
#endif
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
