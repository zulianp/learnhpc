#include "ilp_bench.h"

void handle_align_err() {
  fprintf(stderr, "Expected aligned array!\n");
  exit(EXIT_FAILURE);
}

real_t accumulate(const real_t *x, const ptrdiff_t n) {
  if (((uintptr_t)x % ALIGN_SIZE) != 0) {
    handle_align_err();
  }

  // const real_t *arr = __builtin_assume_aligned(x, ALIGN_SIZE);
  const real_t *arr = x;

  vreal_t acc = {0.};

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

