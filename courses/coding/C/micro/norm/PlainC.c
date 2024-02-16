#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ilp_bench.h"

real_t norm(const real_t *x, const ptrdiff_t n) {
  real_t acc = (real_t)0;
  for (ptrdiff_t i = 0; i < n; i++) {
    acc += x[i] * x[i];
  }

  return sqrt(acc);
}
