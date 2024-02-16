#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int my_system(const char *command){
    pid_t pid = fork();
    if(command == NULL){
        exit(1);
    }
    if (pid == 0) {
    execl("/bin/sh", "sh", "-c", command, (char *) NULL);
    /* execv doesn't return when it works.
    So, if we got here, it failed. */
    perror("execl");
    exit(1);
    } else {
        //This is the parent process
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
    
}