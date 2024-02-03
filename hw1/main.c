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
    bool splitup_malloced = false;
    while (1)
    {
        // read input, call string split to split it up, print out input,  until .
        char* fgets_output = fgets(inp, 4000, stdin);
        if (fgets_output == NULL) {
            exit(EXIT_FAILURE);
        }
        if (strcmp(inp,".\n") == 0)
        {
            break;
        }
        inp[strlen(inp) -1] = '\0';
        splitup = string_split(inp, sep, &num_words);
        splitup_malloced = true;
        // call function
        printf("Num words: %d", num_words);
        for (int i = 0; i < num_words; i++)
        {
            printf("[%s]\n", splitup[i]);
        }
    }
    for (int i = 0; i < num_words; i++)
    {
        free(splitup[i]);
    }
    if(splitup_malloced){
        free(splitup);
    }
    free(inp);
}