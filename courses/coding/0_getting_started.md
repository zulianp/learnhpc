# Getting started

We will use mainly three technologies during this course

1. The `C` programming language.
2. The `Python` scripting language.
3. The Linux/Unix command line by means of `bash` scripts.

Before starting this course make sure you have the following installed in your system:

- A development toolkit (e.g., X-Code command line tools on MacOS, Visual Studio on Windows, ...)
- A `C/C++` compiler (e.g., gcc, clang, or vsc)
- The `Python 3` interpreter

## Installation of Python libraries and virtual environment

In this Section we learn how to:

1. Create a virtual environment (venv).
2. Activate it.
3. Populate it with dependencies.
4. Deactivate it.

The following bash script commands show all the required commands

[create_and_activate_python_environment.sh](https://github.com/zulianp/learnhpc/blob/a8670c99489035695e031d5de04b169d4a3e692c/courses/coding/python/create_and_activate_python_environment.sh#L1-L23)

You can run this script by going to the `learnhpc/courses/coding/python` folder and typing `./create_and_activate_python_environment.sh`. As you can see it will run a bunch of commands and then return. 

If everything went smoothly we can start running python scripts. First we start our venv

```
 source venv/bin/activate 	
```

We run our script and check if we see library and version number
```
./hello.py
```

We can now end our session by entering

```
deactivate
```
