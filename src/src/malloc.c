#include <pthread.h>
#include <sched.h>
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
  const size_t amount_of_blocks = determine_block_size(aligned_size);
  const size_t determined_heap_size =
      determine_heap_size(aligned_size, amount_of_blocks);

  if (!heap || (determined_heap_size > (size_t)SMALL_HEAP_ALLOCATION_SIZE)) {

#ifdef DEBUG
    printf("extending heap\n");
#endif

    block = extend_heap(&heap, aligned_size);
  } else {
    t_heap *tmp_heap = heap;
    size_t block_size = determine_total_block_size(aligned_size);

    while (tmp_heap->next) {
      tmp_heap = tmp_heap->next;
    }

    if ((int64_t)block_size > tmp_heap->free_size) {

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

  if (!block) {
    pthread_mutex_unlock(&mutex);
    return NULL;
  }

  pthread_mutex_unlock(&mutex);
  return (void *)((char *)block + sizeof(t_block));
}
