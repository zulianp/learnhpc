#ifndef ILP_BENCH_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __AVX2__
#include <immintrin.h>
#define ALIGN_SIZE 32
#define ILP_SIZE 2
#endif

#ifdef __ARM_NEON
#include <arm_neon.h>
#define ALIGN_SIZE 64
#define ILP_SIZE 4
#endif

#ifdef DOUBLE_PRECISION
typedef double real_t;

#ifdef __ARM_NEON
  #define DLP_SIZE 2
  typedef float64x2_t vreal_t;
  #define vmovq_n_real vmovq_n_f64
  #define vld1q_real vld1q_f64
  #define vld1_real vld1_f64
  #define vpadds_real vpadds_f64
  #define vaddq_real vaddq_f64
  #define vfmaq_real vfmaq_f64
  #define vgetq_lane_real vgetq_lane_f64
#else
  #define DLP_SIZE 4
  typedef double __attribute__((vector_size(sizeof(double) * DLP_SIZE))) vreal_t;
#endif

inline static void vector_load(const real_t *const a, vreal_t *const v) {
#ifdef __ARM_NEON
  *v = vld1q_f64(a);
#else
#ifdef __AVX2__
  *v = _mm256_load_pd(a);
#else
  *v = __builtin_ia32_loadupd256(a);
#endif
#endif
}

inline static void vector_store(real_t *const a, const vreal_t v) {
#ifdef __ARM_NEON
  #error "IMPLEMENT ME"
#else
#ifdef __AVX2__
  _mm256_store_pd(a, v);
#else
  __builtin_ia32_storeupd256(a, v);
#endif
#endif
}


#else // DOUBLE_PRECISION

typedef float real_t;

#ifdef __ARM_NEON
  #define DLP_SIZE 4
  typedef float32x4_t vreal_t;
  #define vmovq_n_real vmovq_n_f32
  #define vld1q_real vld1q_f32
  #define vld1_real vld1_f32
  #define vpadds_real vpadds_f32
  #define vaddq_real vaddq_f32
  #define vfmaq_real vfmaq_f32
  #define vgetq_lane_real vgetq_lane_f32
#else
  #define DLP_SIZE 8
  typedef float __attribute__((vector_size(sizeof(float) * DLP_SIZE))) vreal_t;
#endif

inline static void vector_load(const real_t *const a, vreal_t *const v) {
#ifdef __ARM_NEON
  *v = vld1q_f32(a);
#else
#ifdef __AVX2__
  *v = _mm256_load_ps(a);
#else
  *v = __builtin_ia32_loadups256(a);
#endif
#endif
}

inline static void vector_store(real_t *const a, const vreal_t v) {
#ifdef __ARM_NEON
  #error "IMPLEMENT ME"
#else
#ifdef __AVX2__
  _mm256_store_ps(a, v);
#else
  __builtin_ia32_storeups256(a, v);
#endif
#endif
}

#endif // DOUBLE_PRECISION
#endif // ILP_BENCH_H
