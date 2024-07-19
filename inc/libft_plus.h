#ifndef LIBFT_PLUS_H
#define LIBFT_PLUS_H

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

#include "alloc.h"

//******** Allocator ********//

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *ft_realloc(void *ptr, size_t size);

/*
 * man malloc: To avoid corruption in multithreaded applications, mutexes
 * are used internally to protect the memory-management data structures
 * employed by these functions.used internally to protect the
 * memory-management data structures employed by these functions.
 */
extern pthread_mutex_t g_mutex;
extern t_heap *g_heap;

//******** Functions ********//

__attribute__((warn_unused_result)) t_block *extend_heap(t_heap **heap,
                                                         size_t size);
__attribute__((warn_unused_result)) t_block *extend_blocks(t_heap **heap,
                                                           size_t size);
size_t list_len(void *lst);
void initialize_heap(t_heap *heap);
t_block *add_block(t_heap **heap, size_t size);

//******** Inline Functions ********//

__attribute__((warn_unused_result)) static inline size_t
determine_amount_blocks(size_t n) {
  size_t amount = n / (SMALL_BLOCK_SIZE - sizeof(t_block));

  if (n % (SMALL_BLOCK_SIZE - sizeof(t_block)) > 0) {
    amount += 1;
  }

  return amount;
}

#endif