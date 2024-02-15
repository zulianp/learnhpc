#include <stddef.h>
#include "ilp_bench.h"

real_t accumulate(const real_t *x, const ptrdiff_t n) {
  real_t acc[ILP_SIZE] = {(real_t)0};

  ptrdiff_t packed_size = (n / (ILP_SIZE * DLP_SIZE)) * (ILP_SIZE * DLP_SIZE);

  for (ptrdiff_t i = 0; i < packed_size; i += (ILP_SIZE * DLP_SIZE)) {
    const real_t *buff = &x[i];

#pragma unroll(ILP_SIZE)
#pragma GCC unroll 2
    for (int i_ilp = 0; i_ilp < ILP_SIZE; i_ilp++) {
      const real_t *buff_simd = &buff[i_ilp * DLP_SIZE];

#pragma unroll(DLP_SIZE)
#pragma GCC unroll 4
      for (int i_dlp = 0; i_dlp < DLP_SIZE; i_dlp++) {
        acc[i_ilp] += buff_simd[i_dlp];
      }
    }
  }

  real_t a = 0;
  for (int i_ilp = 0; i_ilp < ILP_SIZE; i_ilp++) {
    a += acc[i_ilp];
  }

  for (ptrdiff_t i = packed_size; i < n; i++) {
    a += x[i];
  }

  return a;
}
