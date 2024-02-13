#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define REAL_DLP_SIZE 4
typedef double __attribute__((vector_size(sizeof(double) * REAL_DLP_SIZE)))
vreal_t;
#define REAL_ILP_SIZE 2

double accumulate(const double *arr, const ptrdiff_t n) {
  vreal_t acc[REAL_ILP_SIZE];
  for (int k = 0; k < REAL_ILP_SIZE; k++) {
    vreal_t v = {0, 0, 0, 0};
    acc[k] = v;
  }

  ptrdiff_t packed_size =
      (n / (REAL_ILP_SIZE * REAL_DLP_SIZE)) * (REAL_ILP_SIZE * REAL_DLP_SIZE);

  for (ptrdiff_t i = 0; i < packed_size; i += REAL_ILP_SIZE * REAL_DLP_SIZE) {
    const double *buff = &arr[i];

#pragma unroll(REAL_ILP_SIZE)
    for (int k = 0; k < REAL_ILP_SIZE; k += 1) {
      vreal_t *v = (vreal_t *)&(buff[k * REAL_DLP_SIZE]);
      acc[k] += *v;
    }
  }

  double a = 0;
  for (int k = 0; k < REAL_ILP_SIZE; k++) {
    for (int d = 0; d < REAL_DLP_SIZE; d++) {
      a += acc[k][d];
    }
  }

  for (ptrdiff_t i = packed_size; i < n; i++) {
    a += arr[i];
  }

  return a;
}

int main(int argc, char *argv[]) {
  int n = atoi(argv[1]);
  double alpha = atof(argv[2]);

  int alloc_size = ((n / REAL_DLP_SIZE) + 1) * REAL_DLP_SIZE;
  double *arr = (double *)aligned_alloc(REAL_DLP_SIZE * sizeof(double),
                                        alloc_size * sizeof(double));

  assert(arr);

  for (int i = 0; i < n; i++) {
    arr[i] = alpha;
  }

  double a = 0;

  for (int k = 0; k < 40; k++)
    a += accumulate(arr, n);

  printf("%g\n", a);
  free(arr);
  return 0;
}
