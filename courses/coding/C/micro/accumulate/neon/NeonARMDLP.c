#include "ilp_bench.h"

void handle_align_err() {
  fprintf(stderr, "Expected aligned array!\n");
  exit(EXIT_FAILURE);
}

real_t accumulate(const real_t *arr, const ptrdiff_t n) {
  if (((uintptr_t)arr % ALIGN_SIZE) != 0) {
    handle_align_err();
  }

  vreal_t acc = vmovq_n_real(0);

  ptrdiff_t packed_size = (n / (DLP_SIZE)) * (DLP_SIZE);

  for (ptrdiff_t i = 0; i < packed_size; i += DLP_SIZE) {
    vreal_t v = vld1q_real(&arr[i]);
    acc = vaddq_real(v, acc);
  }

  real_t a = 0;

  // Unpack
  #pragma unroll(DLP_SIZE)
  for (int d = 0; d < DLP_SIZE; d++) {
    // a += vgetq_lane_real(acc, d);
    a += acc[d];
  }

  for (ptrdiff_t i = packed_size; i < n; i++) {
    a += arr[i];
  }

  return a;
}
