# shellprogram
Building a Simple Shell (Based on Nutt, Kernel Projects for Linux, Addison-­­Wesley, 2001, Lab #2), or Simple Programming with Files, Processes and Pipes.

## Project Overview

Shell is character-­­oriented, and will fork off processes to execute user commands. The shell read lines of user input into a 256-­­byte buffer, then parse and execute the commands (be sure to clear the buffer between successive commands!). The user specify the command to execute by giving an absolute path to the file containing the executable (e.g. ./hw1); or to use path expansion to locate the file containing the executable by using the environment PATH variable to construct a series of absolute paths and executing the first file found in this way (note that the execvp() command performs this processing automatically, you do not need to program this yourself!) The code  parse the input string and separate it into a collection of sub-­­strings (stored in myargv[]) along with a count of the number of strings encountered (stored in myargc). Note that piped commands will require multiple argc/argv instances!


## Implemenation Strategy to implement a shell

1. Implement shell0 to initialize variables and then go into an infinite loop until stdin detects EOF (i.e. the user enters CTL-­­D). Each iteration through the loop, the shell program should prompt the user for input, read the input & echo it back to the user and then print out another prompt.
2. Implement shell1 by extending shell0 to parse the user’s line of input into a series of strings that are stored into a char myargv ** array, along with a count of the number of strings in an int myargc variable. Print out the contents of myargv and myargc after you have parsed the comment. Allow the user to enter an ‘exit’ command to terminate the shell (instead of typing CTL-­­D and sending a kill signal to terminate execution).
3. Implement shell2 by extending shell1 to create a child process to execute the command (passing in myargv/myargc as arguments) and then wait for the child process to complete execution.


## Shell Supports the following functions

• Execute a single command with up to four command line arguments
(including command line arguments with associated flags). For example: Myshell> ls –l
Myshell> cat myfile
Myshell> ls –al /usr/src/linux

• Execute a command in background. For example:
Myshell> ls -­­l &
Myshell> ls –al /usr/src/linux &

• Redirect the standard output of a command to a file. For example:
Myshell> ls -­­l > outfile
Myshell> ls -­­l >> outfile
Myshell> ls –al /usr/src/linux > outfile2 Myshell> ls –al /usr/src/linux >>outfile2

• Redirect the standard input of a command to come from a file. For example: Myshell> grep disk < outfile
Myshell> grep linux < outfile2

• Execute multiple commands connected by a single shell pipe. For example:
Myshell> ls –al /usr/src/linux | grep linux

• Execute the cd and pwd commands
Myshell> cd some_path
Myshell> pwd


## Building Instructions
1.  Open terminal
2.  Change directory to the folder "shellprogram" using cd and than cd to "myshell.c" 
3.  Compile the myshell.c file using "gcc myshell.c -o hw"

## Run Instructions
1. Run the program using "./hw"

