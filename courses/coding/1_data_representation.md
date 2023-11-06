# Data representation and manipulation

In computational science and machine learning you can find many file formats used to organize and describe complex data and structures. When it comes to numerical simulations common formats are `.vtk` ([Visualization Toolkit](https://vtk.org/)), `.e` ([ExodusII](https://www.osti.gov/servlets/purl/10102115)). In machine learning common formats are `.pt` ([PyTorch](https://pytorch.org/)), ...

In this course we will **NOT** use such formats. We instead handle the binary array data directly.
This choice is motivated by the following reasons

- **Simplicity**. We make our own simple formats. We do not want to spend time in understanding complex data-structures and depend on too many external libraries.
- **Efficiency**. Binary data is exactly the same on disk and in your memory (RAM). We can have minimal input/output code that is very efficient.
- **Debugging**. Being able to simply read/write any data to disk is quite handy for inspecting and debugging.
- **Interoperability**. Data is easily shared between different runtimes and languages.

## Basic resources

A complete table of types in `C` can be found [HERE](https://en.wikipedia.org/wiki/C_data_types).

A complete table of types in `numpy` and `C` can be found [HERE](https://numpy.org/doc/stable/user/basics.types.html).


## Arrays

[Arrays](https://en.wikipedia.org/wiki/Array_(data_structure)) will be the basic data-structure which we will use. Any other data-structure will be a collection of arrays and associated [metadata](https://en.wikipedia.org/wiki/Metadata).

Let us open our console and lets go to `learnhpc/courses/coding/bash`.
Run the example script `./types.sh`. 

The [types.sh](https://github.com/zulianp/learnhpc/blob/2cadfddb36433f2abc7a6c16434fcd547dc8deed/courses/coding/bash/types.sh) script, in combination with [types.py](https://github.com/zulianp/learnhpc/blob/2cadfddb36433f2abc7a6c16434fcd547dc8deed/courses/coding/python/types.py) and [types.c](https://github.com/zulianp/learnhpc/blob/2cadfddb36433f2abc7a6c16434fcd547dc8deed/courses/coding/C/types.c), shows how we can write and read arrays from `Python`, `C`, and inspect them with `bash`. 

### Exercise

1) Understand the content of the files `types.{sh,py,c}`.

2) Prepare one `bash` script, one `C` program, and one `Python` script with the following functionalities: The `bash` script should run the entire exercise without additional user input. The workflow is the following:

1. With `Python` Create an array `x` of type `double` (`float64`) of length `9` with entries `x[i] = i*2`, where `i` is in range `[0, 9)`. Write the array to disk.
2. With `C` read the array and apply the following filter `x[i] = x[i]/2`. Write the array to disk.
3. Use `od` to print the array content.
4. Read the two arrays with python and plot them using `matplotlib`, save the plot to a `png` image.
 
 **HINT**. Most of the functions and commands can be found in `types.{sh,py,c}`, except for `matplotlib` (use Google or your favorite search engine).
