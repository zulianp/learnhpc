#include <inttypes.h> // PRId32, PRId64
#include <stdint.h> // int32_t, int64_t
#include <stdio.h>  // stderr
#include <stdlib.h>

static void *read_array(const char *path, const size_t element_size,
                        const size_t n_elements) {

  // Allocate memory
  void *ptr = malloc(n_elements * element_size);

  // Extra: For write use "wb" instead
  FILE *f = fopen(path, "rb");
  if (!f) {
    // File not found
    fprintf(stderr, "Could not open file: %s\n", path);

    // Not a good practice, but we leave it here for brevity
    exit(EXIT_FAILURE);
  }

  // Extra: For write use fwrite instead
  size_t n_elements_read = fread(ptr, element_size, n_elements, f);

  fclose(f);

  if (n_elements_read != n_elements) {
    // We make sure that we are reading the correct file by checking sizes
    fprintf(stderr, "File %s has wrong length. %lu != %lu\n", path, n_elements,
            n_elements_read);

    // Not a good practice, but we leave it here for brevity
    exit(EXIT_FAILURE);
  }

  return ptr;
}

int main(int argc, const char *argv[]) {
  if (argc > 1) {
    fprintf(stderr, "usage %s\n", argv[0]);
    fprintf(stderr, "No arguments allowed!\n");
  }

  // We know that we are going to read arrays of length 3
  const size_t n = 3;

  int32_t *array_int32 = read_array("array.int32.raw", sizeof(int32_t), n);
  int64_t *array_int64 = read_array("array.int64.raw", sizeof(int64_t), n);
  float *array_float32 = read_array("array.float32.raw", sizeof(float), n);
  double *array_float64 = read_array("array.float64.raw", sizeof(double), n);

  {
    // Print the content of the 4 arrays

    printf("Content of array.int32.raw: [");
    for (size_t i = 0; i < n; i++) {
      printf("%" PRId32 " ", array_int32[i]);
    }
    printf("]\n");

    printf("Content of array.int64.raw: [");
    for (size_t i = 0; i < n; i++) {
      printf("%" PRId64 " ", array_int64[i]);
    }
    printf("]\n");

    printf("Content of array.float32.raw: [");
    for (size_t i = 0; i < n; i++) {
      printf("%f ", array_float32[i]);
    }
    printf("]\n");

    printf("Content of array.float64.raw: [");
    for (size_t i = 0; i < n; i++) {
      printf("%g ", array_float64[i]);
    }
    printf("]\n");
  }

  {
    // Free memory (In C there is no automatic memory management. You have to
    // free what you allocate).

    free(array_int32);
    free(array_int64);
    free(array_float32);
    free(array_float64);
  }

  // Succesful execution is comunicated to our bash script here
  return EXIT_SUCCESS;
}
