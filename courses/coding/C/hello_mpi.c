#include <stdio.h>
#include <stdlib.h>

#include <mpi.h> // Everything MPI is included here

int main(int argc, char *argv[]) {
  if (argc > 1) {
    fprintf(stderr, "usage %s\n", argv[0]);
    fprintf(stderr, "No arguments allowed!\n");
  }

  MPI_Init(&argc, &argv);

  MPI_Comm comm = MPI_COMM_WORLD;
  
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  double x = rank;

  printf("%g\n", x);

  MPI_Finalize();
  return EXIT_SUCCESS;
}
