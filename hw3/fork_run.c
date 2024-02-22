#define _XOPEN_SOURCE 700
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

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
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        /* function not shown */
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        /* in parent process, read from pipe */
        close(write_fd);
        // need to implement this read thing
        //  char* buffer = (char*)malloc(4096);
        //  size_t bytes_read;
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
    // multiple child processes executing at the same time
    // use for loop to handle child processes
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0)
        perror("pipe error"); /* e. g. out of file descriptors */
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t pid[count];
    char *buffer = NULL;
    for (int i = 0; i < count; i++)
    {
        pid[i] = fork();
        if (pid[i] == 0)
        {
            /* in child process, write to pipe */
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(read_fd);
            close(write_fd);
            int argv_len;
            for (int i = 0; argv_base[i] != "/0"; i++)
            {
                argv_len++;
            }
            const char **temper[argv_len + 2];
            for (int j = 0; j < argv_len; j++)
            {
                // figure out how to place all argv base into temp, and index the number, and add null at the end
                temper[j] = argv_base[j];
            }

            // appending number of index that was converted to str to the end
            char index_str[10];
            // convert i value from int to string
            snprintf(index_str, sizeof(index_str), "%d", i);
            temper[argv_len] = index_str;

            // appending null pointer to the end
            temper[argv_len + 1] = "\0";
            execv(argv_base[0], (char **)temper);
            // free(pid);
            exit(EXIT_SUCCESS);
        }

        // printf("%d\n", i);

        // char *temp = NULL;
        // size_t temp_length = 0;
        // size_t buffer_length = 0;
        // FILE *theFile = fdopen(read_fd, "r");
        // size_t len = 0;
        // while (getdelim(&temp, &temp_length, '\0', theFile) == -1)
        // {
        //     temp = getdelim(&temp, &temp_length, '\0', theFile);
        //     len++;
        //     buffer = realloc(buffer, buffer_length + len);
        //     strncat(buffer + len, temp, temp_length);
        //     len += temp_length;
        //     temp = NULL;
        //     temp_length = 0;
        // }
    }

    // for (int i = 0; i < count; i++)
    // {
        if (pid > 0)
        {
            char *buffer = NULL;
            size_t buffer_length = 0;
            FILE *theFile = fdopen(read_fd, "r");
            close(write_fd);
            getdelim(&buffer, &buffer_length, '\0', theFile);
            // printf("%d\n", 2);
            fclose(theFile);
            close(read_fd);
            waitpid(pid, NULL, 0);
        }
    //}

    return buffer;
}