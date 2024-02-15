#include <stddef.h>
#include "ilp_bench.h"

real_t accumulate(const real_t *x, const ptrdiff_t n) {
  real_t acc = (real_t)0;

  ptrdiff_t packed_size = (n / ILP_SIZE) * ILP_SIZE;

  for (ptrdiff_t i = 0; i < packed_size; i += ILP_SIZE) {
    const real_t *buff = &x[i];

#pragma unroll(ILP_SIZE)
#pragma GCC unroll 4
    for (int d = 0; d < ILP_SIZE; d++) {
      acc += buff[d];
    }
  }

  for (ptrdiff_t i = packed_size; i < n; i++) {
    acc += x[i];
  }

  return acc;
}
