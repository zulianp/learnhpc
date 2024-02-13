#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int n = atoi(argv[1]);
  double alpha = atof(argv[2]);

  double *arr = (double *)malloc(n * sizeof(double));

  for (int i = 0; i < n; i++) {
    arr[i] = alpha;
  }

  // BEGIN: Accumulator

  double a = 0;

  for (int k = 0; k < 40; k++) {

    double acc = 0;
    for (int i = 0; i < n; i++) {
      acc += arr[i];
    }
    a += acc;
  }

  // END: Accumulator

  printf("%g\n", a);
  free(arr);
  return 0;
}
