
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

// #ifdef __ARM_FEATURE_SVE

// real_t accumulate(const real_t *arr, const int64_t n) {
//   svbool_t predicate;
//   svfloat64_t v;
//   svfloat64_t acc = svdup_f64_m(0.0);

//   uint64_t vector_length = svlen_f64(acc);
//   for (int64_t i = 0; i < n; i += vector_length) {
//     predicate = svwhilelt_b64(i, n);
//     v = svld1_f64(predicate, &arr[i]);
//     acc = svadd_f64_z(predicate, acc, v);
//   }
//   return svaddv_f64(svptrue_b64(), acc);
//   return rand();
// }

// #else

#ifdef __ARM_NEON

real_t accumulate(const real_t *arr, const ptrdiff_t n) {
  if (((uintptr_t)arr % ALIGN_SIZE) != 0) {
    handle_align_err();
  }

  vreal_t acc = vmovq_n_real(0);

  ptrdiff_t packed_size = (n / (DLP_SIZE)) * (DLP_SIZE);

  for (ptrdiff_t i = 0; i < packed_size; i += DLP_SIZE) {
    const real_t *buff = &arr[i];
    vreal_t v = vld1q_real(buff);
    acc = vaddq_real(v, acc);
  }

  real_t a = 0;

  for (int d = 0; d < DLP_SIZE; d++) {
    a += acc[d];
  }

  for (ptrdiff_t i = packed_size; i < n; i++) {
    a += arr[i];
  }

  return a;
}

#else

real_t accumulate(const real_t *x, const ptrdiff_t n) {
  if (((uintptr_t)x % ALIGN_SIZE) != 0) {
    handle_align_err();
  }

  const real_t *arr = __builtin_assume_aligned(x, ALIGN_SIZE);

  vreal_t acc = {0., 0., 0., 0.};

  ptrdiff_t packed_size = (n / (DLP_SIZE)) * (DLP_SIZE);

  for (ptrdiff_t i = 0; i < packed_size; i += DLP_SIZE) {
    const real_t *buff = &arr[i];
    vreal_t *v = (vreal_t *)buff;
    acc += *v;
  }

  real_t a = 0;
  for (int d = 0; d < DLP_SIZE; d++) {
    a += acc[d];
  }

  for (ptrdiff_t i = packed_size; i < n; i++) {
    a += arr[i];
  }

  return a;
}

#endif

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
