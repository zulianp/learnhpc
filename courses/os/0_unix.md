# Unix system basics

We will be using the console/terminal during this course. In this module we see some pointers to most basic knowledge.

## File system organization

We have to main locations in our filesystem
- Root `/` is the top directory of our filesystem. Enter `cd /` and `ls -lah` to see what is inside. We typically do not want to write here.
- Home directory `~` (`$HOME`). Our base of operations. We can go the the home directory with the following commands, which are equivalent:

- `cd `
- `cd ~`
- `cd $HOME`

Enter `ls -lah` to see what is inside this folder. See `man ls` for extra information.

## Environment

Shell supports the definition of variables. For instance,

```bash
my_var="CIAO"
```
we can access its value with `$my_var`, and print it to the console

```bash
echo $my_var
```
If we want to add variables to the environment we can do so with
```bash
export my_var
```
or directly with
```bash
export my_var="CIAO"
```

Enter `env` to see which variables are defined in our environment.

We can define many usefull environemnt variables inside your `.bashrc`, `.profile`, `.zprofile`, `.zsrch` which can be found in the home folder.

You can augment your shell experience with [https://ohmyz.sh/](https://ohmyz.sh).
This environment allows the use of several plugins, which can be added to the `.zsrch` file. 
Search for the line `plugins=(git)` and add the plug-ins you want. For instance:

`plugins=(git docker sublime sublime-merge gitignore)`
 