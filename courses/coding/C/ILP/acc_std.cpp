// acc_std.cpp
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stddef.h>
#include <vector>

int main(int argc, char *argv[]) {
  ptrdiff_t n = atol(argv[1]);

  std::vector<double> arr(n, 1);

  std::ifstream is(argv[2]);
  if (!is.good()) {
    std::cerr << "Unable to read " << argv[2] << std::endl;
    return EXIT_FAILURE;
  }

  is.read((char *)arr.data(), arr.size() * sizeof(double));
  is.close();

  // BEGIN: Accumulator
  double a = 0;

  for (int k = 0; k < 100; k++) {
    a += std::accumulate(arr.begin(), arr.end(), 0.);
  }
  // END: Accumulator

  printf("result=%g\n", a);

  return 0;
}
