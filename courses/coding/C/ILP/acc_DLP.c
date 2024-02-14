
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DLP_SIZE 4
typedef double __attribute__((vector_size(sizeof(double) * DLP_SIZE))) vreal_t;

static void *read_array(const char *path, const size_t element_size,
                        const size_t n_elements) {

  // Allocate memory (ALIGNED)
  ptrdiff_t alloc_size = ((n_elements / DLP_SIZE) + 1) * DLP_SIZE;
  void *ptr = aligned_alloc(DLP_SIZE * element_size, alloc_size * element_size);

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

void handle_align_err() {
  fprintf(stderr, "Expected aligned array!\n");
  exit(EXIT_FAILURE);
}

double accumulate(const double *arr, const ptrdiff_t n) {
  if (((uintptr_t)arr % sizeof(double) * DLP_SIZE) != 0) {
    handle_align_err();
  }

  vreal_t acc = {0, 0, 0, 0};

  ptrdiff_t packed_size = (n / (DLP_SIZE)) * (DLP_SIZE);

  for (ptrdiff_t i = 0; i < packed_size; i += DLP_SIZE) {
    const double *buff = &arr[i];
    vreal_t *v = (vreal_t *)buff;
    acc += *v;
  }

  double a = 0;
  for (int d = 0; d < DLP_SIZE; d++) {
    a += acc[d];
  }

  for (ptrdiff_t i = packed_size; i < n; i++) {
    a += arr[i];
  }

  return a;
}

int main(int argc, char *argv[]) {
  ptrdiff_t n = atol(argv[1]);
  double *arr = read_array(argv[2], sizeof(double), n);

  assert(arr);

  double a = 0;

  for (ptrdiff_t k = 0; k < 100; k++) {
    a += accumulate(arr, n);
  }

  printf("result=%g\n", a);
  free(arr);
  return 0;
}
