#ifdef __APPLE__
#include <malloc/malloc.h>
#endif

#ifdef linux
#include <malloc.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "libft_plus.h"

int32_t main() {
  char *ptr = ft_malloc(sizeof(char) * 320 + 1);

  for (int i = 0; i < 26; i++) {
    ptr[i] = i + 'A';
  }

  ptr[26] = '\0';
  printf("Malloc: %s\n", ptr);

#ifdef linux
  size = malloc_usable_size(test2);
  printf("Allocated memory size of ft_malloc: %zu bytes\n", size);
#endif /* ifdef linux */

#ifdef __APPLE__
  size_t size = malloc_size(ptr);
  printf("Allocated memory size of ft_malloc: %zu bytes\n", size);
#endif /* ifdef linux */

  return 0;
}
