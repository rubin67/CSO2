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
//clock time
int signalInt = 1;
long long nanosecs() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000000000 + t.tv_nsec;
}

static void handler(int signum)
{

    if (signum == SIGUSR1)
    {
        signalInt = 0;
    }
   
}

void emptyFunc()
{
}

int main(int argc, char *argv[ ] )
{
    long value;
    long endValue;
    if (argc == 1)
    {
        value = nanosecs();
        emptyFunc();
        endValue = nanosecs();
        return endValue - value;
    }
    else if (argc == 2)
    {
        value = nanosecs();
        getpid();
        endValue = nanosecs();
        return endValue - value;
    }
    else if (argc == 3)
    {
        value = nanosecs();
        system("/bin/true");
        endValue = nanosecs();
        return endValue - value;
    }
    else if (argc == 4)
    {
        value = nanosecs();
        kill(getpid(), SIGINT);
        endValue = nanosecs();
        return endValue - value;
    }
    else if (argc == 5)
    {
        value = nanosecs();
        while(signalInt){
            struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000000};
            nanosleep(&ts, NULL);
            handler(SIGUSR1);
        }
        endValue = nanosecs();
        return endValue - value;
    }
}



    //wait to get input of process save the prcoess in the integer 
    //call kill for the otherpid,
    //have first program take eachother's pid, then once have eachother's pid, 
    //process1 send kill to process2, process2 catches that in their handler and
    // then sends a kill back, 
    //have a var in both functions so it runs infiite while loop while it happens otherwise use sigwait
