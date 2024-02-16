
#include "ilp_bench.h"

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
      vector_load(&buff[k * DLP_SIZE], &v);
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
