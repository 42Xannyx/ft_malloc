#include <complex.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"
#include "shared.h"

void report_corruption(void) {
  const char *error_msg = "Memory corruption detected!\n";
  write(STDERR_FILENO, error_msg, 30);
}

int32_t destroy_heap(t_heap **heap) {

  DEBUG_PRINT_SIMPLE("Call munmap()\n");

  int32_t ret = munmap(*heap, (*heap)->total_size);

  return ret;
}

t_heap *find_heap_for_block(t_block *block) {
  t_heap *current_heap = g_heap;

  while (current_heap) {
    uintptr_t heap_start = (uintptr_t)current_heap;
    uintptr_t heap_end = heap_start + current_heap->total_size;

    DEBUG_PRINT("Heap start: %zu\n", heap_start);
    DEBUG_PRINT("Heap start: %zu\n", heap_end);
    DEBUG_PRINT("Block: %zu\n", (uintptr_t)block);

    if ((uintptr_t)block >= heap_start && (uintptr_t)block < heap_end) {
      return current_heap;
    }
    current_heap = current_heap->prev;
  }
  return NULL;
}

void realign_heap(t_heap *to_remove) {
  if (!to_remove->next && !to_remove->prev) {
    g_heap = NULL;
    return;
  }

  if (to_remove->prev) {
    to_remove->prev->next = to_remove->next;
  }

  if (to_remove->next) {
    to_remove->next->prev = to_remove->prev;
  }
}

/**
 * @brief Marks a previously allocated memory block as free.
 *
 * This function doesn't always return memory to the system. Instead, it marks
 * the block as available for future allocations. The memory remains accessible
 * until it's reallocated, so users must set their pointers to NULL after
 * calling this function to prevent accidental use of freed memory.
 *
 * @param ptr Pointer to the memory block to be freed. If NULL, the function
 * does nothing.
 *
 * @note This function checks for heap overflows before marking memory as free.
 *       If an overflow is detected, the program will abort.
 *
 * @warning Using memory after it has been freed can lead to undefined behavior.
 *
 * @return void
 */
void ft_free(void *ptr) {
  pthread_mutex_lock(&g_mutex);

  if (!ptr) {
    write(STDERR_FILENO, "ptr empty\n", 11);

    pthread_mutex_unlock(&g_mutex);
    return;
  }

  if (!g_heap) {
    write(STDERR_FILENO, "heap is empty\n", 15);

    pthread_mutex_unlock(&g_mutex);
    return;
  }

  t_block *block = (t_block *)((char *)ptr - sizeof(t_block));
  t_heap *heap = find_heap_for_block(block);

  if (!heap) {
    write(STDERR_FILENO, "block not found in any heap\n", 28);
    pthread_mutex_unlock(&g_mutex);
    return;
  }

  check_buffer_overflow(block);

  ssize_t current_id = block->_id;
  t_block *tmp = block;

  while (tmp && tmp->_id == current_id) {
    tmp->inuse = false;
    tmp = tmp->next;
  }

  block = tmp;

  size_t block_len = block_list_len_by_id(block);
  bool blocks_in_use = blocks_inuse(heap);

#ifdef DEBUG
  printf("----- FT_FREE ------\n");

  print_heap(heap, false);
#endif

  if (block_len == heap->block_count || blocks_in_use == false) {
    realign_heap(heap);
    int32_t ret = destroy_heap(&heap);

    if (ret != 0) {
      fprintf(stderr, "Failed to destroy heap: %s\n", strerror(errno));

      pthread_mutex_unlock(&g_mutex);
      return;
    }

    /* Does not change anything for the user. This is just out of safety*/
    ptr = NULL;
    block = NULL;
    heap = NULL;

    pthread_mutex_unlock(&g_mutex);
    return;
  }

  pthread_mutex_unlock(&g_mutex);

  return;
}
