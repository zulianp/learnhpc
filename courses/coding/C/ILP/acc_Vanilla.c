#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ilp_bench.h"

static void *read_array(const char *path, const size_t element_size,
                        const size_t n_elements) {

  // Allocate memory
  void *ptr = malloc(n_elements * element_size);

  // Extra: For write use "wb" instead
  FILE *f = fopen(path, "rb");
  if (!f) {
    // File not found
    fprintf(stderr, "Could not open file: %s\n", path);

    // Not a good practice, but we leave it here for brevity
    exit(EXIT_FAILURE);
  }

  // Extra: For write use fwrite instead
  size_t n_elements_read = fread(ptr, element_size, n_elements, f);

  fclose(f);

  if (n_elements_read != n_elements) {
    // We make sure that we are reading the correct file by checking sizes
    fprintf(stderr, "File %s has wrong length. %lu != %lu\n", path, n_elements,
            n_elements_read);

    // Not a good practice, but we leave it here for brevity
    exit(EXIT_FAILURE);
  }

  return ptr;
}

int main(int argc, char *argv[]) {
  ptrdiff_t n = atol(argv[1]);
  real_t *arr = read_array(argv[2], sizeof(real_t), n);

  // BEGIN: Accumulator

  real_t a = (real_t)0;

  for (ptrdiff_t k = 0; k < 100; k++) {

    real_t acc = (real_t)0;
    for (ptrdiff_t i = 0; i < n; i++) {
      acc += arr[i];
    }
    a += acc;
  }

  // END: Accumulator

  printf("result=%g\n", a);
  free(arr);
  return 0;
}
