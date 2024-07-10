#include "debug.h"
#ifdef __APPLE__
#include <malloc/malloc.h>
#endif

#ifdef linux
#include <malloc.h>
#endif

#include "libft_plus.h"

#include <assert.h>
#include <string.h>

/*void ft_malloc_usable_size(void *ptr) { t_block *block = ((t_block *)ptr) - 1;
 * }*/

void stress_test(void) {
  for (int i = 0; i < 126; i++) {
    ft_malloc(sizeof(char) * 1);
  }
}

void fill_string(char *ptr, int64_t n) {
  for (int i = 0; i < n; i++) {
    ptr[i] = (i % 26) + 'A';
  }

  ptr[n] = '\0';
}

int32_t main(void) {
  assert(ft_malloc(0) == NULL);

  char *ptr = ft_malloc(sizeof(char) * 10);
  fill_string(ptr, 79);

  ft_free(ptr);

  printf("(void *) = %p\n", (void *)ptr);

  /*ptr = ft_malloc(sizeof(char) * 10);*/

  /*char *anot = ft_malloc(sizeof(char) * 18723812738);*/

  /*(void)anot;*/

  /*stress_test();*/

  /*fill_string(another_ptr, 500);*/

  /*printf("Malloc: %s\n", ptr);*/
  /*printf("strlen: %ld\n", strlen(ptr));*/

  /*printf("Malloc: %s\n", another_ptr);*/
  /*printf("strlen: %ld\n", strlen(another_ptr));*/

  return 0;
}
