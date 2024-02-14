#ifndef ILP_BENCH_H

#define ILP_SIZE 2

#ifdef DOUBLE_PRECISION

typedef double real_t;

#ifdef __ARM_NEON 
#include <arm_neon.h>
#define DLP_SIZE 2
typedef float64x2_t vreal_t;
#define vmovq_n_real vmovq_n_f64 
#define vld1q_real vld1q_f64
#define vpadds_real vpadds_f64
#define vaddq_real vaddq_f64 
#else

#define DLP_SIZE 4
typedef double __attribute__((vector_size(sizeof(double) * DLP_SIZE))) vreal_t;
#endif

#else // DOUBLE_PRECISION

typedef float real_t;

#ifdef __ARM_NEON 
#include <arm_neon.h>
#define DLP_SIZE 4
typedef float32x4_t vreal_t;
#define vmovq_n_real vmovq_n_f32 
#define vld1q_real vld1q_f32
#define vpadds_real vpadds_f32
#define vaddq_real vaddq_f32
#else

#define DLP_SIZE 8
typedef float __attribute__((vector_size(sizeof(float) * DLP_SIZE))) vreal_t;
#endif


 
#endif //DOUBLE_PRECISION
#endif //ILP_BENCH_H