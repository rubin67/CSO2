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
        perror("pipe error"); /* e. g. out of file descriptors */
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t pid = fork();

    if (pid == 0)
    {
        /* in child process, write to pipe */
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(read_fd);
        close(write_fd);
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
        /* function not shown */
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0){
        /* in parent process, read from pipe */
        close(write_fd);
        //need to implement this read thing
        // char* buffer = (char*)malloc(4096);
        // size_t bytes_read;
        char *buffer = NULL; 
        size_t buffer_length = 0;
        FILE *theFile = fdopen(read_fd, "r");

        getdelim(&buffer, &buffer_length, '\0', theFile);
        fclose(theFile);
        waitpid(child_pid, NULL, 0);
        return buffer;
    }
    else
    return 0;
}
char *parallelgetoutput(int count, const char **argv_base)
{
    //multiple child processes executing at the same time
    //use for loop to handle child processes
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0)
        perror("pipe error"); /* e. g. out of file descriptors */
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t *pid = (malloc)(count * sizeof(pid_t));
    
    for(int i = 0; i < count; i++){
    pid[i] = fork();
    if (pid[i] == 0)
    {
        /* in child process, write to pipe */
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(read_fd);
        close(write_fd);
        

        //const char** temp = (char**)malloc(sizeof(char*)*sizeof(argv_base+2));
        const char** temper = malloc(sizeof(char*) * (sizeof(argv_base) + 2));
        char index_str[10];
        snprintf(index_str, sizeof(index_str), "%d",i);
        for(int j = 0; j < sizeof(argv_base); j++){
        //figure out how to place all argv base into temp, and index the number, and add null at the end
        temper[j] = argv_base[j];
            
        }
        temper[sizeof(argv_base)] = index_str;
        temper[sizeof(argv_base)+1] = "\0";
        
        //handle pointer to pointer as opposed to just pointer
        execv(argv_base[0], (char **)temper);
        //free(pid);
        exit(EXIT_SUCCESS);
        
    }
    else if (pid[i] > 0){
        waitpid(pid[i], NULL, 0);
        printf("%d",i);
        //free(pid);
    }
        free(pid);
    }
        char *buffer = NULL;
        char *temp = NULL;
        size_t temp_length = 0;
        size_t buffer_length = 0;
        FILE *theFile = fdopen(read_fd, "r");
        size_t len = 0;
    while(getdelim(&temp, &temp_length, '\0', theFile) != -1){
        len++;
        buffer = realloc(buffer, buffer_length + len);
        strncat(buffer+len, temp, temp_length);
        len += temp_length;
        free(temp);
        temp = NULL;
        temp_length = 0;
    }
    // if(temp){
    //     free(temp);
    // }

    return buffer;
}