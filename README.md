# Assignment 3 - RShell

## Introduction
The basic overview of our design is as follows:

1. The shell will first print a command prompt.

2. The shell will then read in a line of command(s) and connector(s) from the standard input. The line read in will be in this format:

							`$ ls -a && cd lab-01`
							
The `ls -a` represents the first command being executed, the `cd lab-01` represents the second command being executed and the `&&` represents the connector which connects the first command on the left and the second command on the right, which will execute the second based on what connector is used.

3. The appropriate commands are then executed using `fork()`, `execvp()`, and `waitpid()`.

## Release History
`hw3 v.1.1` : Released Dec 3, 2018

`hw3 v.1.0` : Released Nov 29, 2018.

`hw2` : Released Nov 21, 2018.


## Known Bugs
`hw3 v.1.1`
- Although precedence does work, using the bracket `[ ]` notation for test is somewhat working. For example, an input such `echo hello ; [ -e rshell ]` will run and execute both left and right sides, however, an input such as `[ -e rshell ] ; echo hello` will execute the left side twice and ignore the right side completely.

`hw3 v.1.0`
- RShell builds and runs sucessfully however it does not work for precedence cases and this is because our logic is flawed and we did not have enough time to fix this error.

`hw2`
- `void parse()` was not parsing the user input properly. For example, if `ls -a` was an input, it would push each individual character in its own element rather than each individual string. As a result, the arguments sent into `execvp()` were not being executed because it didn't recognize the contents of the argument vector as valid arguments.
