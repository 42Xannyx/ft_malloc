#include <complex.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "alloc.h"
#include "debug.h"
#include "libft_plus.h"
#include "shared.h"

void report_corruption(void) {
  const char *error_msg = "Memory corruption detected!\n";
  write(STDERR_FILENO, error_msg, 30);
}

int32_t destroy_heap(t_heap **heap) {

#ifdef DEBUG
  printf("Call munmap()\n");
#endif

  int32_t ret = munmap(*heap, (*heap)->total_size);

  return ret;
}

void ft_free(void *ptr) {
  pthread_mutex_lock(&g_mutex);

  if (!ptr) {
    fprintf(stderr, "ptr empty\n");

    pthread_mutex_unlock(&g_mutex);
    return;
  }

  if (!g_heap) {
    fprintf(stderr, "heap is empty\n");

    pthread_mutex_unlock(&g_mutex);
    return;
  }

  t_block *block = (t_block *)((char *)ptr - sizeof(t_block));
  t_heap *heap = g_heap;

#ifdef DEBUG
  printf("----- FT_FREE ------\n");
  /*print_block(block);*/
#endif

  check_block_integrity(block);

  size_t block_len = list_len(block);
  if (block_len == heap->block_count) {
    int32_t ret = destroy_heap(&heap);

    if (ret != 0) {
      // TODO: Should I add an error messsage?
      fprintf(stderr, "Failed to destroy heap: %s\n", strerror(errno));

      pthread_mutex_unlock(&g_mutex);
      return;
    }

    ptr = NULL;   /* Does not change anything for the user. This is just out
                     of safety*/
    block = NULL; /* Does not change anything for the user. This is just out
                   of safety*/

    // Set Global heap to NULL to show it does exist
    heap = NULL;
    g_heap = heap;

#ifdef DEBUG
    printf("BLOCK\n");
    if (block) {
      print_block(block);
    }

    if (heap) {
      print_heap(heap, false);
    }
#endif

    pthread_mutex_unlock(&g_mutex);
    return;
  }

  block->inuse = false;

  pthread_mutex_unlock(&g_mutex);
  return;
}
