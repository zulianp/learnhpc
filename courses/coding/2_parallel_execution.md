# Parallel execution

In this module we cover the basic concepts of parallelism and run our first and very simple parallel codes.

## Types of parallelism

**Instruction-level parallelism (ILP)** is a form of parallelism that is exploited by the compiler and hardware to execute multiple instructions in parallel. ILP is achieved through techniques such as pipelining, branch prediction, and speculative execution.

**Thread-level parallelism (TLP)** is a form of parallelism that is achieved by running multiple threads in parallel. Threads are lightweight processes that share the same memory space. TLP is often used to improve the performance of applications that are limited by the single-thread performance of a processor.

**Data-level parallelism (DLP)** is a form of parallelism that is achieved by operating on multiple data elements in parallel. DLP is often used in conjunction with other forms of parallelism, such as TLP, to improve the performance of applications that are data-intensive.

**Supercomputing** is the use of high-performance computers to solve problems that are too large or complex for ordinary computers. Supercomputers are typically used by scientists, engineers, and researchers to solve problems in fields such as weather forecasting, climate modeling, and molecular biology.

Here is a table summarizing the key differences between the different types of parallelism:

| Type of parallelism | Focus | Techniques | Examples |
|---|---|---|---|
| ILP | Instructions | Pipelining, branch prediction, speculative execution | Your laptop's CPU |
| TLP | Threads | Multithreading | Hyper-threading, multicore processors |
| DLP | Data | SIMD, vectorization | Intel SSE, AVX |
| Supercomputing | Large-scale problems | Distributed computing, parallel processing | CSCS Piz Daint |


## OpenMP

OpenMP is an application programming interface (API) that provides a set of compiler directives and library routines for parallelizing programs written in C, C++, and Fortran. OpenMP is a high-level programming model that allows programmers to explicitly specify which parts of their code should be executed in parallel and how the parallel tasks should be distributed among the available threads or processors.

### Scope of OpenMP

OpenMP is limited to user-directed parallelization. This means that the programmer must explicitly specify which parts of their code should be executed in parallel. The compiler and runtime system will not automatically parallelize any code that is not explicitly marked as parallel.

OpenMP is designed for shared-memory parallel computers. This means that all of the threads in a parallel region must have access to the same shared memory space. OpenMP does not support distributed-memory parallel computers.

OpenMP is a fork-join programming model. This means that a single thread creates a team of threads to execute a parallel region, and then waits for all of the threads to finish before continuing.

OpenMP provides a number of directives for controlling the behavior of parallel regions, including:

- `#pragma omp parallel`: This directive marks the beginning of a parallel region.
- `#pragma omp for`: This directive parallelizes a for loop.
- `#pragma omp update`: This directive updates shared memory locations in a parallel region.

OpenMP also provides a number of library routines which we do not cover here.


### Installing and first usage of OpenMP

For this module we need to install `OpenMP`. OpenMP can be installed with package managers or from [source](https://github.com/OpenMP).

Let us assume we installed `OpenMP` from source and compile our example `learnhpc/courses/coding/C/hello_openmp.c`.
First define the environment variable `OPENMP_DIR` to the `OpenMP` install location. The compile with

```bash
cc -Xpreprocessor -fopenmp -I$OPENMP_DIR/include -L$OPENMP_DIR/lib -lomp hello_openmp.c -o hello_openmp.exe
```

Let us run the executable with 8 threads. If our CPU has less or more cores, then we change the number of threads accordingly.

```bash
OMP_NUM_THREADS=8 OMP_PROC_BIND=true ./hello_openmp.exe
```

We should see numbers from 0 to 7 printed out of order to the terminal.

## MPI

Message Passing Interface (MPI) is a standardized, open-source communication API that enables efficient message passing between processors on a parallel computer or between distributed computers. MPI is used by scientists, engineers, and researchers in various fields to develop and run parallel and distributed applications.

### Scope of MPI

MPI provides a set of functions for communication between processes. These functions can be used to send and receive messages, create and manage groups of processes, and perform other communication tasks. MPI also provides a number of collective communication routines that can be used to perform operations on all of the processes in a group, such as broadcast and reduce.

MPI is a portable library that can be used on a wide variety of parallel and distributed computing systems. It is also relatively easy to learn and use, making it a popular choice for parallel computing applications.

### Installing and first usage of MPI

Install `MPI` using your favorite package manager.

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

## OpenMP VS MPI

Here is a summary of the differences between OpenMP and MPI.

| Feature | MPI | OpenMP |
|---|---|---|
| Communication model  | **Point-to-point message passing** | **Shared memory** |
| Scalability | **Scalable to large numbers of processors** | **Scalable to large numbers of threads** |
| Programming model | **Explicit** | **Implicit** |
| Complexity | **More complex** | **Easier to learn and use** |
| Typical applications | **Large-scale scientific or engineering problems** | **Smaller-scale problems** |


MPI is a good choice for applications that need to scale to large numbers of processors and that require low-level control over communication. OpenMP is a good choice for applications that are already written in C, C++, or Fortran and that need to be parallelized quickly and easily.

## Exercise

Describe the difference between the two technologies utilizing the `hello_{openmp,mpi}.c` examples.
