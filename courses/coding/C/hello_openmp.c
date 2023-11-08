#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc > 1) {
    fprintf(stderr, "usage %s\n", argv[0]);
    fprintf(stderr, "No arguments allowed!\n");
  }

  size_t n = 8;
  double *x = malloc(n * sizeof(double));

  // Initialize our array in parallel, for instance with x[i] = i
#pragma omp parallel
  {
#pragma omp for
    for (size_t i = 0; i < n; i++) {
      x[i] = i;
    }
  }

  // Print the content of our array also in parallel just to say hi!
#pragma omp parallel
  {
#pragma omp for
    for (size_t i = 0; i < n; i++) {
      printf("%g\n", x[i]);
    }
  }

  free(x);
  return EXIT_SUCCESS;
}
