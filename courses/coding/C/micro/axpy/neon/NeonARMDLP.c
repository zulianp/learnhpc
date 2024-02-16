#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ilp_bench.h"

void handle_align_err() {
  fprintf(stderr, "Expected aligned array!\n");
  exit(EXIT_FAILURE);
}

void axpy(real_t *__restrict__ l, const real_t *__restrict__ r,
          const ptrdiff_t n) {
  if (((uintptr_t)l % ALIGN_SIZE) != 0) {
    handle_align_err();
  }

  if (((uintptr_t)r % ALIGN_SIZE) != 0) {
    handle_align_err();
  }

  ptrdiff_t packed_size = (n / (DLP_SIZE)) * (DLP_SIZE);

  vreal_t alpha = vmovq_n_real(1);
  vreal_t left[ILP_SIZE];
  vreal_t right[ILP_SIZE]; 

//   vreal_t l0, l1;
//   vreal_t r0, r1;
// #ifndef DOUBLE_PRECISION
//   vreal_t l2, l3;
//   vreal_t r2, r3;
// #endif

  for (ptrdiff_t i = 0; i < packed_size; i += DLP_SIZE) {
    real_t *l_buff = &l[i];
    const real_t *r_buff = &r[i];

#pragma unroll(DLP_SIZE)
    for (int d = 0, i_ilp = 0; d < ILP_SIZE; d++) {
      left[d] = vld1q_real(&l_buff[i_ilp]);
      right[d] = vld1q_real(&r_buff[i_ilp]);
      left[d] = vfmaq_real(left[d], alpha, right[d]);

      l_buff[i_ilp + 0] = vgetq_lane_real(left[d], 0);
      l_buff[i_ilp + 1] = vgetq_lane_real(left[d], 1);

#ifndef DOUBLE_PRECISION
      l_buff[i_ilp + 2] = vgetq_lane_real(left[d], 2);
      l_buff[i_ilp + 3] = vgetq_lane_real(left[d], 3);
#endif

       i_ilp += DLP_SIZE;
    }

    //           l0 = vld1q_real(&l_buff[0 * DLP_SIZE]);
    //           r0 = vld1q_real(&r_buff[0 * DLP_SIZE]);
    //           l0 = vfmaq_real(l0, alpha, r0);

    //           l1 = vld1q_real(&l_buff[1 * DLP_SIZE]);
    //           r1 = vld1q_real(&r_buff[1 * DLP_SIZE]);
    //           l1 = vfmaq_real(l1, alpha, r1);

    // #ifndef DOUBLE_PRECISION
    //           l2 = vld1q_real(&l_buff[2 * DLP_SIZE]);
    //           r2 = vld1q_real(&r_buff[2 * DLP_SIZE]);
    //           l2 = vfmaq_real(l2, alpha, r2);

    //           l3 = vld1q_real(&l_buff[3 * DLP_SIZE]);
    //           r3 = vld1q_real(&r_buff[3 * DLP_SIZE]);
    //           l3 = vfmaq_real(l3, alpha, r3);
    // #endif

    //           l_buff[0 * DLP_SIZE] = vgetq_lane_real(l0, 0);
    //           l_buff[0 * DLP_SIZE + 1] = vgetq_lane_real(l0, 1);
    //           l_buff[1 * DLP_SIZE] = vgetq_lane_real(l1, 0);
    //           l_buff[1 * DLP_SIZE + 1] = vgetq_lane_real(l1, 1);

    // #ifndef DOUBLE_PRECISION
    //           l_buff[2 * DLP_SIZE] = vgetq_lane_real(l2, 0);
    //           l_buff[2 * DLP_SIZE + 1] = vgetq_lane_real(l2, 1);
    //           l_buff[3 * DLP_SIZE] = vgetq_lane_real(l3, 0);
    //           l_buff[3 * DLP_SIZE + 1] = vgetq_lane_real(l3, 1);
    // #endif

  }
}
