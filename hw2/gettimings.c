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

    if (signum == SIGUSR1)
    {
        raise(SIGUSR2);
        
    }
    else if(signum == SIGUSR2){
        __asm__("");
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
        emptyFunc();
        endValue = nanosecs();
        printf("%lu", (endValue - value - extra));
    }
    else if (atoi(argv[1]) == 2)
    {
        value = nanosecs();
        getpid();
        endValue = nanosecs();
        printf("%lu", (endValue - value - extra));
    }
    else if (atoi(argv[1]) == 3)
    {
        value = nanosecs();
        system("/bin/true");
        endValue = nanosecs();
        printf("%lu", (endValue - value - extra));
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
        printf("%lu", (endValue - value - extra));
    }
    else if (atoi(argv[1]) == 5)
    {
        struct sigaction sa;
        sa.sa_handler = handlerReply;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sigaction(SIGUSR1, &sa, NULL);
        sigaction(SIGUSR2, &sa, NULL);
        value = nanosecs();
        raise(SIGUSR1);
        struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000000};
        nanosleep(&ts, NULL);
        endValue = nanosecs();
        printf("%lu", (endValue - value - extra));
    }
}
