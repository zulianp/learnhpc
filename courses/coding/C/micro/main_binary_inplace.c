#include "utils.h"
#include <string.h>

#ifndef compute_kernel
#error "define compute_kernel"
#endif

void compute_kernel(real_t *l, const real_t *r, const ptrdiff_t n);

int main(int argc, char *argv[]) {
  if(argc < 4) {
    fprintf(stderr, "usage %s <n> <data.float64.raw> <repeat>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const ptrdiff_t n = atol(argv[1]);
  real_t *left = (real_t *)read_array(argv[2], sizeof(real_t), n);
  real_t *right = (real_t *)read_array(argv[2], sizeof(real_t), n);
  const ptrdiff_t repeat = atol(argv[3]);

  assert(left);
  assert(right);

  double time_spent = 0;

  for (ptrdiff_t k = 0; k < repeat; k++) {
    clock_t begin = clock();
    compute_kernel(left, right, n);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    memcpy(left, right, n * sizeof(real_t));
  }

  // to check
  real_t a = 0;
  for (ptrdiff_t i = 0; i < n; i++) {
    a += left[i];
  }

  free(left);
  free(right);

  printf("#----------------------\n");
  printf("Executable:\t%s\n", argv[0]);
  printf("Problem-size:\t%ld\n", n);
  printf("Repeat:\t\t%ld\n", repeat);
  printf("Throughput:\t%g [GB/s]\n", 1e-9 * (2 * n * repeat)/time_spent);
  printf("TTS:\t\t%g [ms]\n", 1001*(time_spent / repeat));
  printf("Result:\t\t%g\n", a);
  printf("#----------------------\n");
  return 0;
}
