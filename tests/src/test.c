/*#ifdef __APPLE__*/
/*#include <malloc/malloc.h>*/
/*#endif*/
/**/
/*#ifdef linux*/
/*#include <malloc.h>*/
/*#endif*/

#include "libft_plus.h"

#include <string.h>

void fill_string(char *ptr, int64_t n) {
  for (int i = 0; i < n; i++) {
    ptr[i] = (i % 26) + 'A';
  }
}

int32_t main() {
  char *test = ft_malloc(3);

  printf("test = %s\n", test);

  char *ptr = ft_malloc(sizeof(char) * 320);
  char *another_ptr = ft_malloc(320);

  // Should overflow
  fill_string(ptr, 319);

  ptr[319] = '\0';
  another_ptr[319] = '\0';
  printf("Malloc: %s\n", ptr);
  printf("strlen: %ld\n", strlen(ptr));

  printf("Malloc: %s\n", another_ptr);
  printf("strlen: %ld\n", strlen(another_ptr));

#ifdef linux
  size = malloc_usable_size(test2);
  printf("Allocated memory size of ft_malloc: %zu bytes\n", size);
#endif /* ifdef linux */

#ifdef __APPLE__
  /*size_t size = malloc_size(ptr);*/
  /*printf("Allocated memory size of ft_malloc: %zu bytes\n", size);*/
#endif /* ifdef linux */

  return 0;
}
