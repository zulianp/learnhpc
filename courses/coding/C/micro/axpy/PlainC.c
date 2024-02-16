#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ilp_bench.h"

void axpy(real_t *__restrict__ l, const real_t *__restrict__ r,
          const ptrdiff_t n) {
  const real_t alpha = l[n-1];
  for (ptrdiff_t i = 0; i < n; i++) {
    l[i] += alpha * r[i];
  }
}
