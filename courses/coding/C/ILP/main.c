
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

real_t accumulate(const real_t *x, const ptrdiff_t n);

int main(int argc, char *argv[]) {
  if(argc < 4) {
    fprintf(stderr, "usage %s <n> <data.float64.raw> <repeat>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const ptrdiff_t n = atol(argv[1]);
  real_t *arr = (real_t *)read_array(argv[2], sizeof(real_t), n);
  const ptrdiff_t repeat = atol(argv[3]);

  assert(arr);

  clock_t begin = clock();

  real_t a = 0;
  for (ptrdiff_t k = 0; k < repeat; k++) {
    a += accumulate(arr, n);
  }

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  free(arr);

  printf("#----------------------\n");
  printf("Executable:\t%s\n", argv[0]);
  printf("Problem-size:\t%ld\n", n);
  printf("Repeat:\t\t%ld\n", repeat);
  printf("Throughput:\t%g [GB/s]\n", 1e-9 * (n * repeat)/time_spent);
  printf("TTS:\t\t%g [s]\n", time_spent);
  printf("Result:\t\t%g\n", a);
  printf("#----------------------\n");
  return 0;
}
