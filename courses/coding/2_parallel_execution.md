# Parallel execution

In this module we run our first and very simple parallel codes.

## OpenMp

For this module we need to install `OpenMP`. OpenMP can be installed with package managers or from [source](https://github.com/OpenMP).

Let us assume we installed `OpenMP` from source and compile our example `learnhpc/courses/coding/C/hello_openmp.c`.
First define the environment variable `OPENMP_DIR` to the `OpenMP` install location. The compile with

```bash
cc -Xpreprocessor -fopenmp -I$OPENMP_DIR/include -L$OPENMP_DIR/lib -lomp hello_openmp.c -o hello_openmp.exe
```

Let us run the executable with 8 threads. If your CPU has less or more cores change the number of threads accordingly.

```bash
OMP_NUM_THREADS=8 OMP_PROC_BIND=true ./hello_openmp.exe
```

We should see numbers from 0 to 7 printed out of order to the terminal.

## MPI

Let us check if we have the mpi executables accessible and where they are with
```bash
which mpicc mpiexec
```

Let us compile our example at `learnhpc/courses/coding/C/hello_mpi.c`

```bash
mpicc hello_mpi.c -o hello_mpi.exe
```
Let us run our example with 8 MPI processes

```bash
mpiexec -np 8 ./hello_mpi.exe
```

Again, we should see numbers from 0 to 7 printed out of order to the terminal.


## Exercise

Describe the difference between the two technologies utilizing the `hello_{openmp,mpi}.c` examples.
