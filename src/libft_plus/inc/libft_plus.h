#ifndef LIBFT_PLUS_H
#define LIBFT_PLUS_H

#include <stdlib.h>
#include <unistd.h>

#include "alloc.h"

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *ft_realloc(void *ptr, size_t size);

//******** Utils ********//

__attribute__((warn_unused_result)) t_block *extend_heap(t_heap *heap,
                                                         size_t size);
void initialize_heap(t_heap *heap);

#endif
