# Getting started

We will use mainly four technologies during this course

1. The `C` programming language.
2. The `Python` scripting language.
3. The Linux/Unix command line by means of `bash` scripts and basic command line tools.
4. Git

Before starting any module make sure you have the following installed in your system:

- A development toolkit (e.g., X-Code command line tools on MacOS, Visual Studio on Windows, ...)
- A `C/C++` compiler (e.g., gcc, clang, or vsc)
- The `Python 3` interpreter
- Git

First thing first, clone this repository on your machine

```bash
git clone https://github.com/zulianp/learnhpc.git
```

If you already did make sure that you have the latest version 

```bash
cd learnhpc
git pull
```

## Installation of `C` dependencies

We need the following technologies

- [OpenMP](https://www.openmp.org/)
- [CUDA](https://developer.nvidia.com/cuda-downloads)
- Any variant of the MPI standard ([MPICH](https://www.mpich.org/), [OpenMPI](https://www.open-mpi.org/))

All these technologies can be installed by means of package managers.
For instance 

- Homebrew or MacPorts on MacOS
- Pacman, Yum, Apt on Linux platforms

## Installation of Python dependencies and virtual environment

In this Section we see how to:

1. Create a virtual environment (venv).
2. Activate it.
3. Populate it with dependencies.
4. Deactivate it.

The following bash script shows all the required commands

[create_and_activate_python_environment.sh](https://github.com/zulianp/learnhpc/blob/a8670c99489035695e031d5de04b169d4a3e692c/courses/coding/python/create_and_activate_python_environment.sh#L1-L23)

You can run this script by going to the `learnhpc/courses/coding/python` folder and typing 

```bash
./create_and_activate_python_environment.sh
``` 

As you can see it will run a bunch of commands and then return. 

If everything went smoothly we can start running python scripts. First we start our venv

```bash
 source venv/bin/activate 	
```

We run our script and check if we see library and version number

```bash
python3 hello.py
```

We can now end our session by entering

```bash
deactivate
```

More information can be found [here](https://docs.python.org/3/library/venv.html).