# Parallel execution

In this module we run our first and very simple parallel codes.

## OpenMp

OpenMP is an application programming interface (API) that provides a set of compiler directives and library routines for parallelizing programs written in C, C++, and Fortran. OpenMP is a high-level programming model that allows programmers to explicitly specify which parts of their code should be executed in parallel and how the parallel tasks should be distributed among the available threads or processors.

### Scope of OpenMP:

OpenMP is limited to user-directed parallelization. This means that the programmer must explicitly specify which parts of their code should be executed in parallel. The compiler and runtime system will not automatically parallelize any code that is not explicitly marked as parallel.

OpenMP is designed for shared-memory parallel computers. This means that all of the threads in a parallel region must have access to the same shared memory space. OpenMP does not support distributed-memory parallel computers.

OpenMP is a fork-join programming model. This means that a single thread creates a team of threads to execute a parallel region, and then waits for all of the threads to finish before continuing. OpenMP does not support other programming models, such as data parallelism or task parallelism.

OpenMP provides a number of directives for controlling the behavior of parallel regions, including:

- `#pragma omp parallel`: This directive marks the beginning of a parallel region.
- `#pragma omp for`: This directive parallelizes a for loop.
- `#pragma omp update`: This directive updates shared memory locations in a parallel region.
- `#pragma omp sections`: This directive divides a parallel region into a number of sections, each of which is executed by a different thread.
- `#pragma omp critical`: This directive protects a critical section of code from being executed by multiple threads simultaneously.
- `#pragma omp barrier`: This directive creates a barrier that all threads in a parallel region must wait for before continuing.

OpenMP also provides a number of library routines for performing tasks such as:

Creating and destroying teams of threads
Setting the number of threads in a team
Getting the current thread ID
Synchronizing threads

### Installing and first usage of OpenMP

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
