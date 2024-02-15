# Understanding Instruction Level Parallelism (ILP) with C Code Examples

## Objective:
By the end of this lesson, students should be able to:
1. Define Instruction Level Parallelism (ILP) and understand its significance in enhancing processor performance.
2. Identify opportunities for ILP in code and recognize dependencies that may limit ILP.
3. Analyze C code examples to illustrate ILP concepts and techniques for maximizing parallelism.

## Introduction to ILP:
Instruction Level Parallelism (ILP) refers to the ability of a processor to execute multiple instructions simultaneously, thereby increasing throughput and overall performance. ILP is achieved by identifying independent instructions within a program and executing them concurrently. Modern processors employ various techniques to exploit ILP, such as instruction reordering, pipeline execution, and out-of-order execution.

### Key Concepts:
1. **Dependency:** Instructions are considered dependent if one instruction relies on the result produced by another instruction. Dependencies can limit the degree of ILP achievable in a program.
2. **Types of Dependencies:**
   - **Data Dependency:** Occurs when an instruction requires the result produced by a previous instruction.
   - **Control Dependency:** Arises from conditional branches in the code, where the execution of subsequent instructions depends on the outcome of a branch.
3. **Techniques for Exploiting ILP:**
   - **Instruction Scheduling:** Reordering instructions to maximize parallel execution while respecting dependencies.
   - **Loop Unrolling:** Duplicating loop iterations to expose more parallelism.
   - **Compiler Optimizations:** Utilizing compiler optimizations to identify and exploit ILP.

## C Code Examples:

### Example 1: No explicit ILP

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    int n = atoi(argv[1]);
    double alpha = atof(argv[2]);

    double * arr = (double *)malloc(n * sizeof(double));

    for(int i = 0; i < n; i++) {
        arr[i] = alpha * i;
    }
    
    // BEGIN: Accumulator

    double acc = 0;
    for(int i = 0; i < n; i++) {
        acc += arr[i];
    }

    // END: Accumulator

    printf("%g\n", acc);
    free(arr);
    return 0;
}

```

In this example, we do not 


```c

#include <stdio.h>
#include <stdlib.h>

#define ILP_SIZE 4

int main(int argc, char * argv[]) {
    int n = atoi(argv[1]);
    double alpha = atof(argv[2]);

    double * arr = (double *)malloc(n * sizeof(double));

    for(int i = 0; i < n; i++) {
        arr[i] = alpha * i;
    }
    
    // BEGIN: Accumulator

    double acc[ILP_SIZE] = {0};
    for(int i = 0; i < n; i += ILP_SIZE) {
        #pragma unroll(ILP_SIZE)
        for(int k = 0; k < ILP_SIZE; k++) {
            acc[k] += arr[i+k];
        }
    }

    double a = 0;
      for(int k = 0; k < ILP_SIZE; k++) {
        a += acc[k];
      }

    // END: Accumulator

    printf("%g\n", a);
    free(arr);
    return 0;
}

```

In this example, the loop in the compute function can be parallelized as each iteration operates independently on different array elements.

## Conclusion
Instruction Level Parallelism (ILP) is crucial for improving processor performance by executing multiple instructions simultaneously. By identifying and exploiting independent instructions, programmers can enhance the efficiency of their code. Through techniques like instruction scheduling, loop unrolling, and compiler optimizations, ILP can be maximized, leading to better performance in computing tasks.


