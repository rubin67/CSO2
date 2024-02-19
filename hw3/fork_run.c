#define _XOPEN_SOURCE 700
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>




char *getoutput(const char *command)
{
    pid_t child_pid = getpid();
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0)
        handle_error(); /* e. g. out of file descriptors */
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t pid = fork();

    if (child_pid == 0)
    {
        /* in child process, write to pipe */
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(read_fd);
        close(write_fd);
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        /* function not shown */
        exit(EXIT_SUCCESS);
    }
    else if (child_pid > 0){
        /* in parent process, read from pipe */
        close(write_fd);
        //need to implement this read thing
        char* buffer = (char*)malloc(4096);
        
        read(read_fd, buffer, sizeof(buffer));

        close(read_fd);
        waitpid(child_pid, NULL, 0);
    } else {
        return -1;
    }
}
char *parallelgetoutput(int count, const char **argv_base)
{
    //multiple child processes executing at the same time
    //use for loop to handle child processes
    for(int i = 0; i <= count; i++){
    pid_t pid = fork();
    int child_pid = getpid();
    if (child_pid == 0)
    {
        char** temp = (char**)malloc(sizeof(char*)*sizeof(argv_base+2));
        printf(0);
        execv(temp[0], temp);
        //figure out how to place all argv base into temp, and index the number, and add null at the end
    }
    else if (child_pid > 0){
        waitpid(child_pid, NULL, 0);
        printf(i);
    }
    }
}