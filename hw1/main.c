#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include "split.h"
int main(int argc, char *argv[])
{
    char *sep;
    if (argc < 2)
    {
        sep = " \t";
    }
    else
    {
        size_t totalSize = 0;
        for (int i = 1; i < argc; i++)
        {
            totalSize += strlen(argv[i]);
        }

        sep = (char *)malloc(totalSize + 1);

        sep[0] = '\0'; // Ensure the string is initially empty
        for (int i = 1; i < argc; i++)
        {
            strcat(sep, argv[i]);
        }
    }

    int num_words;
    char *inp = (char *)malloc(4000);
    char **splitup;
    while (1)
    {
        // read input, call string split to split it up, print out input,  until .
        fgets(inp, 4000, stdin);
        if (fgets(inp, 4000, stdin) == NULL) {
        exit(EXIT_FAILURE);
        }
        if (strcmp(inp,".\n") == 0)
        {
            break;
        }
        inp[strlen(inp) -1] = '\0';
        splitup = string_split(inp, sep, &num_words);
        // call function
        for (int i = 0; i < num_words; i++)
        {
            printf("[%s]\n", splitup[i]);
        }
    }
    for (int i = 0; i < num_words; i++)
    {
        free(splitup[i]);
    }
    free(splitup);
    free(inp);
}
