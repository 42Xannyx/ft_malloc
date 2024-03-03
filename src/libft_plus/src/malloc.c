#include <stdio.h>
#include <sys/mman.h>

void *ft_malloc(size_t size) {
  void *value = NULL;

  const allocated_value =
      (void *)mmap(value, size, PROT_WRITE | PROT_READ, 0, 0, 0);

  return allocated_value;
}
