// acc_std.cpp
#include <numeric>
#include <stddef.h>
#include <vector>

#include "ilp_bench.h"

extern "C" real_t accumulate(const real_t *x, const ptrdiff_t n) {
  real_t init = 0;
  return std::accumulate(x, x + n, init);
}
