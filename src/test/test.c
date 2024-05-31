#include <malloc.h>
#include <stdint.h>
#include <stdio.h>

#include "libft_plus.h"

int32_t main() {
  char *test2 = ft_malloc(sizeof(char) * 0);
  char *test1 = malloc(sizeof(char) * 0);

  // for (int i = 0; i < 20; i++) {
  //   test1[i] = 'A'; // Fill with 'A'
  //   test2[i] = 'A'; // Fill with 'A'
  // }
  // test1[20] = '\0'; // Null-terminator
  // printf("Malloc test1: %s\n", test1);
  //
  // test2[20] = '\0'; // Null-terminator
  // printf("Ft_malloc test1: %s\n", test2);

  size_t size = malloc_usable_size(test1);
  printf("Allocated memory size of malloc: %zu bytes\n", size);

  size = malloc_usable_size(test2);
  printf("Allocated memory size of ft_malloc: %zu bytes\n", size);

  return 0;
}
