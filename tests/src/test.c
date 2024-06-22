#ifdef __APPLE__
#include <malloc/malloc.h>
#endif

#ifdef linux
#include <malloc.h>
#endif

#include "libft_plus.h"

#include <assert.h>
#include <string.h>

void fill_string(char *ptr, int64_t n) {
  for (int i = 0; i < n; i++) {
    ptr[i] = (i % 26) + 'A';
  }

  ptr[n] = '\0';
}

int32_t main(void) {
  assert(ft_malloc(0) == NULL);

  char *ptr = ft_malloc(sizeof(char) * 320);
  char *another_ptr = ft_malloc(sizeof(char) * 512);

  // Should overflow
  fill_string(ptr, 10);
  fill_string(another_ptr, 500);

  printf("Malloc: %s\n", ptr);
  printf("strlen: %ld\n", strlen(ptr));

  printf("Malloc: %s\n", another_ptr);
  printf("strlen: %ld\n", strlen(another_ptr));

#ifdef linux
  /*size = malloc_usable_size(ptr);*/
  /*printf("Allocated memory size of ft_malloc: %zu bytes\n", size);*/
#endif /* ifdef linux */

#ifdef __APPLE__
  /*size_t size = malloc_size(ptr);*/
  /*printf("Allocated memory size of ft_malloc: %zu bytes\n", size);*/
#endif /* ifdef linux */

  return 0;
}
