#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ilp_bench.h"

real_t accumulate(const real_t *x, const ptrdiff_t n) {
  real_t a = (real_t)0;

  for (ptrdiff_t k = 0; k < 100; k++) {

    real_t acc = (real_t)0;
    for (ptrdiff_t i = 0; i < n; i++) {
      acc += x[i];
    }
    a += acc;
  }

  return a;
}
