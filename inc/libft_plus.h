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

__attribute__((warn_unused_result)) t_block *extend_heap(t_heap **, t_amount,
                                                         size_t);
__attribute__((warn_unused_result)) t_block *extend_blocks(t_heap **, size_t);

size_t block_list_len(void *lst);
size_t block_list_len_by_id(void *lst);
void initialize_heap(t_heap *heap);
t_block *add_block(t_heap **heap, size_t size);
t_block *reuse_block(t_heap **heap, size_t size);

//******** Inline Functions ********//

__attribute__((warn_unused_result)) static inline size_t
get_amount_blocks(t_amount n) {
  return n.tiny + n.small;
}

#endif
