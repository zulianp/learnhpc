
#include "ilp_bench.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>


static void *read_array(const char *path, const size_t element_size,
                        const size_t n_elements) {

  // Allocate memory (ALIGNED)
  ptrdiff_t alloc_size = ((n_elements / ALIGN_SIZE) + 1) * ALIGN_SIZE;
  void *ptr = aligned_alloc(ALIGN_SIZE, alloc_size * element_size);

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

