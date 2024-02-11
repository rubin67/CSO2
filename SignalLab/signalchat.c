#define _XOPEN_SOURCE 700 // request all POSIX features, even with -std=c11
#include <stdlib.h>       // for EXIT_SUCCESS, NULL, abort
#include <stdio.h>        // for getline
#include <unistd.h>       // for getpid
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


char *inbox_data;
char *outbox_data;

char inbox_filename[512];
char outbox_filename[512];


pid_t other_pid = 0;
int size = 4096;

void cleanUp()
{
    munmap(inbox_data, size);
    shm_unlink(inbox_filename);
}

void setupInbox(char *inbox_filename, char **inbox_data)
{
    snprintf(inbox_filename, sizeof inbox_filename, "/%d-chat", getpid());
    int inbox_fd = shm_open(inbox_filename, O_CREAT | O_RDWR, 0666);
    if (inbox_fd < 0){
        exit(1);
    }
    ftruncate(inbox_fd, size);

    *inbox_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, inbox_fd, 0);
    if (*inbox_data == (char *) MAP_FAILED)
    {
        exit(1);
    }

    close(inbox_fd);
}

void setupOutbox(char *outbox_filename, char **outbox_data)
{
    snprintf(outbox_filename, sizeof outbox_filename, "/%d-chat", other_pid);
    int outbox_fd = shm_open(outbox_filename, O_CREAT | O_RDWR, 0666);
    if (outbox_fd < 0){
        exit(1);
    }
    ftruncate(outbox_fd, size);

    *outbox_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, outbox_fd, 0);
    if (*outbox_data == (char *) MAP_FAILED)
    {
        exit(1);
    }

    close(outbox_fd);
}

static void handler(int signum)
{

    if (signum == SIGINT)
    {
        cleanUp();
        kill(other_pid, SIGTERM);
    }

    else if (signum == SIGTERM)
    {
        cleanUp();
    }
    else if (signum == SIGUSR1)
    {        
        fputs(inbox_data,stdout);
        fflush(stdout);
        inbox_data[0] = '\0';
    }
}

int main(void)
{

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);


    printf("This process's ID: %ld\n", (long)getpid());
    char *line = NULL;
    size_t line_length = 0;
    do
    {
        printf("Enter other process ID: ");
        if (-1 == getline(&line, &line_length, stdin))
        {
            perror("getline");
            abort();
        }
    } while ((other_pid = strtol(line, NULL, 10)) == 0);
    free(line);

    setupInbox(inbox_filename, &inbox_data);
    setupOutbox(outbox_filename, &outbox_data);


    while(1){
        fputs(inbox_data, stdout);
        fflush(stdout);
        if(fgets(outbox_data, 4096, stdin) == NULL){
            break;
        }
        kill(other_pid, SIGUSR1);
        while(outbox_data[0]){
            struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000000};
            nanosleep(&ts, NULL);
        }
    }


    return EXIT_SUCCESS;
}