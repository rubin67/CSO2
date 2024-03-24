#define _GNU_SOURCE
#include "util.h"
#include <stdio.h>    // for printf
#include <stdlib.h>   // for atoi (and malloc() which you'll likely use)
#include <sys/mman.h> // for mmap() which you'll likely use
#include <stdalign.h>

alignas(4096) volatile char global_array[4096 * 32];

void labStuff(int which)
{
    if (which == 0)
    {
        /* do nothing */
    }
    else if (which == 1)
    {
        // write to array
        for (int i = 0; i < 4096 * 32; i += 4096)
        {
            global_array[i] = 'h';
        }
    }
    else if (which == 2)
    {
        // allocates 1 MB of memory and access a few bytes of allocation
        // do not change resident set size
        volatile char *stuff = (char *)malloc(1024 * 1024);
    }
    else if (which == 3)
    {
        // increase virtual memory address by 1048576
        // increase resident size by 131072
        for (int i = 0; i < 4096 * 31; i += 4096)
        {
            global_array[i] = 'h';
        }
        char *stuff = (char *)malloc(1024 * 1020);
    }
    else if (which == 4)
    {
        // char *ptr;
        // ptr = mmap(0x5555555bbfff+0x200000,
        //            4096,
        //            PROT_READ | PROT_WRITE,
        //            MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE,
        //            -1, 0);
        // if (ptr == MAP_FAILED)
        // {
        //     handle_error();
        // }
        // ptr = (char*)malloc(4096);
        // 55555555a000-5555555bbfff + 
    }
}

int main(int argc, char **argv)
{
    int which = 0;
    if (argc > 1)
    {
        which = atoi(argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: %s NUMBER\n", argv[0]);
        return 1;
    }
    printf("Memory layout:\n");
    print_maps(stdout);
    printf("\n");
    printf("Initial state:\n");
    force_load();
    struct memory_record r1, r2;
    record_memory_record(&r1);
    print_memory_record(stdout, NULL, &r1);
    printf("---\n");

    printf("Running labStuff(%d)...\n", which);

    labStuff(which);

    printf("---\n");
    printf("Afterwards:\n");
    record_memory_record(&r2);
    print_memory_record(stdout, &r1, &r2);
    print_maps(stdout);
    return 0;
}
