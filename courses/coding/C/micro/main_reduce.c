#include "utils.h"

#ifndef compute_kernel
#error "define compute_kernel"
#endif

real_t compute_kernel(const real_t *x, const ptrdiff_t n);

int main(int argc, char *argv[]) {
  if(argc < 4) {
    fprintf(stderr, "usage %s <n> <data.float64.raw> <repeat>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const ptrdiff_t n = atol(argv[1]);
  real_t *arr = (real_t *)read_array(argv[2], sizeof(real_t), n);
  const ptrdiff_t repeat = atol(argv[3]);

  assert(arr);

  clock_t begin = clock();

  real_t a = 0;
  for (ptrdiff_t k = 0; k < repeat; k++) {
    a += compute_kernel(arr, n);
  }

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  free(arr);

  printf("#----------------------\n");
  printf("Executable:\t%s\n", argv[0]);
  printf("Problem-size:\t%ld\n", n);
  printf("Repeat:\t\t%ld\n", repeat);
  printf("Throughput:\t%g [GB/s]\n", 1e-9 * (n * repeat)/time_spent);
  printf("TTS:\t\t%g [ms]\n", 1001*(time_spent / repeat));
  printf("Result:\t\t%g\n", a);
  printf("#----------------------\n");
  return 0;
}
