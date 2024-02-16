#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <time.h>
#include <stdlib.h> // for EXIT_SUCCESS, NULL, abort
#include <stdio.h>  // for getline
#include <unistd.h> // for getpid
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
    long value;
    long endValue;
    pid_t otherpid;
    pid_t fivePid;

//clock time
long long nanosecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000000000 + t.tv_nsec;
}

static void handler(int signum)
{

    if (signum == SIGUSR1)
    {
    }

   
}
static void handlerReply(int signum)
{
    //printf("coming to handler 5 %d\n", signum);
    if (signum == SIGUSR1)
    {
        //printf("coming to SIGUSR1\n");
    }
    else if(signum == SIGUSR2){
        //printf("coming to SIGUSR2\n");
        endValue = nanosecs();
    }
   
}
static void handlerNegative(int signum)
{

    if (signum == SIGUSR1)
    {
        //printf("-1 handler\n");
        kill(fivePid, SIGUSR2);
        printf("%d\n", fivePid);
        
    }
   
}


__attribute__((noinline))void emptyFunc()
{
    __asm__("");

}


int main(int argc, char *argv[])
{
    int i = 0;
    long extra = 0;
    while(i <= 100){
        value = nanosecs();
        endValue = nanosecs();
        extra += (endValue - value);
        i++;
    }
    extra /= 100;
    if (atoi(argv[1]) == 1)
    {
        
        value = nanosecs();
        int i = 0;
        while(i<=100){
        emptyFunc();
        endValue = nanosecs();
        i++;
        }
        printf("Scenario 1: %lld\n", ((endValue - value)/100 - extra));
    }
    else if (atoi(argv[1]) == 2)
    {
        value = nanosecs();
        int i = 0;
        while(i<=100){
        getpid();
        endValue = nanosecs();
        i++;
        }
        printf("Scenario 2: %lld\n", ((endValue - value)/100 - extra));
    }
    else if (atoi(argv[1]) == 3)
    {
        value = nanosecs();
        int i = 0;
        while(i<=100){
        system("/bin/true");
        endValue = nanosecs();
        i++;
        }
        printf("Scenario 3: %lld\n", ((endValue - value)/100 - extra));
    }
    else if (atoi(argv[1]) == 4)
    {
        struct sigaction sa;
        sa.sa_handler = handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sigaction(SIGUSR1, &sa, NULL);
        value = nanosecs();

        raise(SIGUSR1);
        endValue = nanosecs();
        printf("Scenario 4: %lld\n", (endValue - value - extra));
    }
    else if (atoi(argv[1]) == 5)
    {
        struct sigaction sa;
        sa.sa_handler = handlerReply;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sigaction(SIGUSR1, &sa, NULL);
        sigaction(SIGUSR2, &sa, NULL);
        sigset_t signalSet;
        sigemptyset(&signalSet);
        int sig;


        printf("%d\n", getpid());
        scanf("%d", &otherpid);
        int i = 0;   
        while(i <10){
        value = nanosecs();
        kill(otherpid, SIGUSR1);
        i++;
        }
        
        printf("Scenario 5: %lld\n", ((value - endValue)/100) - extra);
    }
    else if(atoi(argv[1]) == -1){
        
        char *line = NULL;
        size_t line_length = 0;
        printf("%d\n", getpid());
        scanf("%d", &fivePid);
        
        struct sigaction sa;
        sa.sa_handler = handlerNegative;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sigaction(SIGUSR1, &sa, NULL);
        while(1){
            pause();
        }

    }
}

