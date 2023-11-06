# Getting started

We will use mainly five technologies during this course

1. The `C` programming language.
2. [`Makefile`](https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/).
3. The `Python` language.
4. The Linux/Unix command line by means of `bash` scripts and basic command line tools.
5. The [`Git`](https://git-scm.com/) version control system.

All the tutorials will make use of the `console` / `terminal` command line application available in your system.

## Why this combination of technologies

**C programming** is advantageous due to its efficiency, portability across different platforms, scalability for both small and large projects, widespread usage with extensive libraries, and the ability to directly access hardware, making it essential for high-performance code development.

**Makefiles** automate builds, reduce unnecessary compilation, improve project organization, and ensure platform-independent processes, streamlining development tasks.

**Python**'s advantages include clear readability, extensive libraries, platform independence, versatility across applications, and strong community support, making it a popular and effective programming language.

**Bash** is advantageous for scripting tasks, executing system commands, its default presence in Unix systems, and allowing interface customization.

**Git** is essential for version control, enabling collaboration, backup, branching, merging, and benefiting from a vast community, streamlining software development processes.

## First things first

Before starting make sure you have the following installed in your system:

- A development toolkit (e.g., X-Code command line tools on MacOS, Visual Studio on Windows, ...)
- A `C/C++` compiler (e.g., gcc, clang, or vsc)
- The `Python 3` interpreter
- Git

First thing first, clone this repository on your machine

```bash
git clone https://github.com/zulianp/learnhpc.git
```

If you already did, make sure that you have the latest version 

```bash
cd learnhpc
git pull
```

## Installation of `C` dependencies

We need the following technologies

- [OpenMP](https://www.openmp.org/)
- Any variant of the MPI standard ([MPICH](https://www.mpich.org/), [OpenMPI](https://www.open-mpi.org/))
- [CUDA](https://developer.nvidia.com/cuda-downloads) (Optional since you need an NVIDIA graphics card to use it)

All these technologies can be installed by means of package managers.
For instance 

- [Homebrew](https://brew.sh/) or [MacPorts](https://www.macports.org/) on MacOS
- [Pacman](https://wiki.archlinux.org/title/pacman), [Apt](https://linux.die.net/man/8/apt-get), etc.. on Linux platforms

## Installation of `Python` dependencies and virtual environment

Python virtual environments isolate project dependencies, preventing conflicts, simplifying management, ensuring reproducibility, maintaining system cleanliness, and easing deployment.

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

You typical workflow will consist of activating the environment, develop, test, and run your scripts, and finally and eventually deactivate it when you are done.

More information can be found [here](https://docs.python.org/3/library/venv.html).
