#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    const char *argv_base[] = {
        "/bin/echo", "running", NULL
    };
    const char *output = parallelgetoutput(2, argv_base);
    
    printf("Text: [%s]\n", output);
}