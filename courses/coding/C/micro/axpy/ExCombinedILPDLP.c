#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ilp_bench.h"

void handle_align_err() {
  fprintf(stderr, "Expected aligned array!\n");
  exit(EXIT_FAILURE);
}

// inline static
void micro(const vreal_t alpha, real_t *const __restrict__ l_buff,
           const real_t *const __restrict__ r_buff) {
  for (int k = 0; k < ILP_SIZE; k += 1) {
    vreal_t lv, rv;
    vector_load(&l_buff[k * DLP_SIZE], &lv);
    vector_load(&r_buff[k * DLP_SIZE], &rv);

    lv += alpha * rv;

    real_t *const res = &l_buff[k * DLP_SIZE];

    vector_store(res, lv);
  }
}

void axpy(real_t *__restrict__ l, const real_t *__restrict__ r,
          const ptrdiff_t n) {
  if (((uintptr_t)l % ALIGN_SIZE) != 0) {
    handle_align_err();
  }

  if (((uintptr_t)r % ALIGN_SIZE) != 0) {
    handle_align_err();
  }

  vreal_t alpha = {l[n-1]};

  ptrdiff_t packed_size = (n / (ILP_SIZE * DLP_SIZE)) * (ILP_SIZE * DLP_SIZE);

  for (ptrdiff_t i = 0; i < packed_size; i += ILP_SIZE * DLP_SIZE) {
    micro(alpha, &l[i], &r[i]);
  }
  for (ptrdiff_t i = packed_size; i < n; i++) {
    l[i] += r[i];
  }
}
