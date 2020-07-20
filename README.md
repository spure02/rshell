# RShell
### Team Members
- [Simi Purewal](https://github.com/spure02)
- [Ashley McDaniel](https://github.com/ASHTM15)

### Table of Contents
- [Introduction](#introduction)
- [Design](#design)
- [Roadblocks](#roadblocks)
- [Release History](#release-history)
- [Known Bugs](#known-bugs)

## Introduction
The basic overview of our design is as follows.

1. The shell will first print a command prompt.

2. The shell will then read in a line of command(s) and connector(s) from the standard input. The line read in will be in this format:

```
						$ ls -a && cd lab-01
```
							
The `ls -a` represents the first command being executed, the `cd lab-01` represents the second command being executed and the `&&` represents the connector which connects the first command on the left and the second command on the right, which will execute the second based on what connector is used.

3. The appropriate commands are then executed using `fork()`, `execvp()`, and `waitpid()`.

4. The test command will be responsible for a subset of file testing thus allowing the user to run commands using the keyword `test`. The test command will first check that the path to the file exists. The test command will account for `[ ]` as well. The flags are as following: `-e`, `-f`, and `-d`. 

5. The test command will display true or false to the terminal based on how the flag was evaluated.

6. There will be four other connectors which deal with input and output redirection. An example is the following command:

```
		$ cat < existingInputFile | tr A-Z a-z | tee newOutputFile1 | tr a-z A-Z > newOutputFile2
```
The `cat` is executed on `existingInputFile`, then the `A-Z` in `existingInputFile` is changed to `a-z`, then the truncation which occurred previously is outputted to a new file called `newOutputFile1`, then the `a-`z in `newOutputFile1` is changed to `A-Z`, and finally the truncation which occurred previously is outputted to a new file called `newOutputFile2`.

7. These new commands are executed using `dup2() and pipe()`.

## Design
In order to implement this overview, we plan to use the composite pattern design. Starting with a base class called `Base`, we will run the shell through the different composite classes and leaf classes which interconnect and communicate with each other. We also have two aggregate classes called `Shell` and `Parser` which build the program and parse the user input respectively.
![diagram pic](https://i.imgur.com/g0PTnMv.png)

## Roadblocks
Since we are working in git, we will most likely run into merge conflicts when pushing/pulling code to and from GitHub. In order to overcome them, we will be very careful when making commits with careful communication and teamwork so we can avoid them. Another issue we may encounter is the need to change our design as we start programming if we find that it is lacking. We will overcome this by being flexible and able to change our program if the need arises.

## Release History
`hw4 v1.2`: Released Dec 13, 2018

`hw3 v.1.1` : Released Dec 3, 2018

`hw3 v.1.0` : Released Nov 29, 2018.

`hw2` : Released Nov 21, 2018.

## Known Bugs
`hw4 v1.2`
- Chaining the new connectors together with `|` is not working properly. It will create the appropriate `sPipe` object for the first argument, but then the shell stops reading everything after the first `|` and executes everything else. We believe this is because our `parse()` is neglecting anything that is still in `data` while parsing the user input. 

`hw3 v.1.1`
- Although precedence does work, using the bracket `[ ]` notation for test is somewhat working. For example, an input such `echo hello ; [ -e rshell ]` will run and execute both left and right sides, however, an input such as `[ -e rshell ] ; echo hello` will execute the left side twice and ignore the right side completely.

`hw3 v.1.0`
- RShell builds and runs sucessfully however it does not work for precedence cases and this is because our logic is flawed and we did not have enough time to fix this error.

`hw2`
- `void parse()` was not parsing the user input properly. For example, if `ls -a` was an input, it would push each individual character in its own element rather than each individual string. As a result, the arguments sent into `execvp()` were not being executed because it didn't recognize the contents of the argument vector as valid arguments.
