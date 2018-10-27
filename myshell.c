/****************************************************************
 * Name        : Dhruv Shha                                     *
 * Class       : CSC 415                                        *
 * Date        : July 8                                         *
 * Description :  Writting a simple bash shell program          *
 * 	              that will execute simple commands. The main   *
 *                goal of the assignment is working with        *
 *                fork, pipes and exec system calls.            *
 ****************************************************************/
//May add more includes
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

//Max amount allowed to read from input
#define BUFFERSIZE 256
//Shell prompt
#define PROMPT "myShell >> "
//sizeof shell prompt
#define PROMPTSIZE sizeof(PROMPT)

#define ARGVMAX 64
#define PIPECNTMAX 10

int main(int argc, char** argv){
    
    while (1) {
        
        char buffersize[256];
        // buffer for user input
        char *token;
        // gets the next token from the str tok function
        
        
        int checkBackground = 0;
        //boolean variable for checking if there was a &
        int in = 0;
        // boolean variable for checking in file input by the users
        int out = 0;
        // boolean variable for checking out file input by the users
        int out2 = 0;
        // boolean variable for checking appending out2 file input by the users
        printf(PROMPT);
        // prints myshell >>
        
        // pulls the user input and checks for end of file
        if(fgets(buffersize, 256, stdin) == NULL) {
            exit(0);
        }
        
        // tokenazies first token of user input
        token = strtok(buffersize, " \n\t");
        
        char *argsMax1[64];
        // 2d array for holding the excutable shell commands
        char *argsMax2[64];
        // 2d array for holding the excutable shell commands when there is a pipe
        
        
        int argc1 = 0;
        // size of argsMax1
        int argc2 = 0;
        // size of argsMax2
        int checkPipe = 0;
        // boolean variable for checking if there was a pipe
        
        char pwd[256];
        // command for holding the current worrking directory
        
        // holds all the file for redirection commands
        char *infile;
        char *outfile;
        char *outfile2;
        
        while (token != NULL) {
            
            // the while loop tokenaizes all the other user input into argsMax1 and to argsMax2
            if(strcmp(token, "|") == 0){
                //checks for pipe
                checkPipe = 1;
                argsMax1[argc1] = NULL;
                token = strtok(NULL, " \n\t");
            }
            //checks for background
            if(strcmp(token, "&") == 0){
                checkBackground = 1;
                argsMax1[argc1] = NULL;
                break;
            }
            if(strcmp(token, "<") == 0){
                //checks for input for redirection
                in = 1;
                argsMax1[argc1] = NULL;
                token = strtok(NULL, " \n\t");
                infile = token;
                break;
            }
            if(strcmp(token, ">") == 0){
                //checks for output file redirection
                out = 1;
                argsMax1[argc1] = NULL;
                token = strtok(NULL, " \n\t");
                outfile = token;
                break;
            }
            if(strcmp(token, ">>") == 0){
                //checks for output file redirection
                out2 = 1;
                argsMax1[argc1] = NULL;
                token = strtok(NULL, " \n\t");
                outfile2 = token;
                break;
            }
            
            if(!checkPipe){
                argsMax1[argc1] = token;
                argc1++;
                // puts the token for regular commands and for side of the pipe
            } else {
                argsMax2[argc2] = token;
                argc2++;
                // puts the token for the second side of the pipe
            }
            
            token = strtok(NULL, " \n\t");
            // gets next token
        }
        
        // null required for end of array to execute commands
        argsMax1[argc1] = NULL;
        argsMax2[argc2] = NULL;
        
        
        if(strcmp(argsMax1[0], "exit") == 0) {
            //if user types in exit the program ends
            exit(0);
        }
        else if (strcmp(argsMax1[0], "pwd") == 0) {
            // if user types in pwd gives current working directory
            if (getcwd(pwd, sizeof(pwd)) == NULL) {
                perror("Error in getting pathname");
            }
            else {
                printf("%s ", pwd);
            }
        }
        else if (strcmp(argsMax1[0], "cd") == 0) {
            // if user types in cd changes the directory
            chdir(argsMax1[1]);
        }
        else {
            
            if(checkPipe) {
                // pipe execution
                pid_t p1, p2;
                int returnValCheck = 0;
                int p[2];
                
                pipe(p); //creats two sides sub pipes
                
                
                // pipe1
                p1 = fork();
                
                
                if(p1 < 0){
                    perror("Error in fork child");
                    exit(0);
                }
                else if (p1 == 0) {
                    dup2(p[1], STDOUT_FILENO);
                    close(p[0]);
                    
                    returnValCheck = execvp(argsMax1[0], argsMax1);
                    if(returnValCheck < 0) {
                        perror("Error child1");
                        exit(0);
                    }
                }
                
                
                //pipe 2
                p2 = fork();
                if(p2 < 0){
                    perror("Error in fork child ");
                    exit(0);
                }
                else if (p2 == 0) {
                    dup2(p[0], STDIN_FILENO);
                    close(p[1]);
                    
                    returnValCheck = execvp(argsMax2[0], argsMax2);
                    if(returnValCheck < 0) {
                        perror("Error child2");
                        exit(0);
                    }
                }
                close(p[0]);
                close(p[1]);
                
                int s1, s2;
                if(waitpid(p1, &s1, 0) < 0){
                    perror("Error1");
                    exit(0);
                }
                if(waitpid(p2, &s2, 0) < 0){
                    perror("Error2");
                    exit(0);
                }
                
                
            }
            else {
                // executes command
                pid_t pid;
                
                // file discriptors
                int in_file =0;
                int out_file1 =0;
                int out_file2 =0;
                
                // hold file discriptors
                int hold_in = 0;
                int hold_out1 =0;
                int hold_out2 =0;
                
                // checks the value
                int returnValCheck = 0;
                
                
                // changes output and input of commands
                if(in){
                    in_file = open(infile, O_RDONLY);
                    hold_in = dup(STDIN_FILENO);
                    dup2(in_file, STDIN_FILENO);
                    close(in_file);
                } if(out) {
                    out_file1 = open(outfile, O_WRONLY | O_TRUNC | O_CREAT);
                    hold_out1 = dup(STDOUT_FILENO);
                    dup2(out_file1, STDOUT_FILENO);
                    close(out_file1);
                } if(out2) {
                    out_file2 = open(outfile2, O_WRONLY | O_APPEND | O_CREAT);
                    hold_out2 = dup(STDOUT_FILENO);
                    dup2(out_file2, STDOUT_FILENO);
                    close(out_file2);
                }
                
                pid = fork();
                if(pid < 0){
                    perror("Error in fork child");
                    exit(0);
                }
                else if (pid == 0) {
                    returnValCheck = execvp(argsMax1[0], argsMax1);
                    if(returnValCheck < 0) {
                        perror("Error in Command Line");
                    }
                    exit(0);
                }
                else{
                    
                    // changes back original command output input
                    if(in) {
                        dup2(hold_in, STDIN_FILENO);
                        close(hold_in);
                    } if(out){
                        dup2(hold_out1, STDOUT_FILENO);
                        close(hold_out1);
                    } if(out2) {
                        dup2(hold_out2, STDOUT_FILENO);
                        close(hold_out2);
                    }
                    
                    if(!checkBackground){
                        // unless background & is inputed by user, uses wait function
                        int status;
                        if((waitpid(pid, &status, 0)) < 0){
                            perror("Error");
                            exit(0);
                        }
                    }
                }
            }
        }
        printf("\n");
    }
    return 0;
}

