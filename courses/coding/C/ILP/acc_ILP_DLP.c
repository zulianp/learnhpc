
#include "ilp_bench.h"

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

void handle_align_err() {
  fprintf(stderr, "Expected aligned array!\n");
  exit(EXIT_FAILURE);
}

real_t accumulate(const real_t *x, const ptrdiff_t n) {
  if (((uintptr_t)x % ALIGN_SIZE) != 0) {
    handle_align_err();
  }

  // Why is this killing performance?
  // const real_t *arr = __builtin_assume_aligned(x, ALIGN_SIZE);
  const real_t *arr = x;

  vreal_t acc[ILP_SIZE];
  for (int k = 0; k < ILP_SIZE; k++) {
    vreal_t v = {0};
    acc[k] = v;
  }

  ptrdiff_t packed_size = (n / (ILP_SIZE * DLP_SIZE)) * (ILP_SIZE * DLP_SIZE);

  for (ptrdiff_t i = 0; i < packed_size; i += ILP_SIZE * DLP_SIZE) {
    const real_t *buff = &arr[i];

    for (int k = 0; k < ILP_SIZE; k += 1) {
#if 0
      vreal_t *v = (vreal_t *)&(buff[k * DLP_SIZE]);
      acc[k] += *v;
#else
      vreal_t v;
      vector_copy(&buff[k * DLP_SIZE], &v);
      acc[k] += v;
#endif
    }
  }

  real_t a = 0;
  for (int k = 0; k < ILP_SIZE; k++) {
    for (int d = 0; d < DLP_SIZE; d++) {
      a += acc[k][d];
    }
  }

  for (ptrdiff_t i = packed_size; i < n; i++) {
    a += arr[i];
  }

  return a;
}

int main(int argc, char *argv[]) {
  ptrdiff_t n = atol(argv[1]);
  real_t *arr = read_array(argv[2], sizeof(real_t), n);

  assert(arr);

  real_t a = 0;

  for (ptrdiff_t k = 0; k < 100; k++) {
    a += accumulate(arr, n);
  }

  printf("result=%g\n", a);
  free(arr);
  return 0;
}
