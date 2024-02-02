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
    else{
    size_t totalSize = 0;
    for (int i = 1; i < argc; i++) {
        totalSize += strlen(argv[i]);
    }

    sep = (char *)malloc(totalSize + 1);

    sep[0] = '\0'; // Ensure the string is initially empty
    for (int i = 1; i < argc; i++) {
        strcat(sep, argv[i]);
    }

    }  

    int num_words;
    char *inp = (char *)malloc(4000);
    char **splitup;
    while (1)
    {
        // read input, call string split to split it up, print out input,  until .
        fgets(inp, sizeof(inp), stdin);
        if (fgets(inp, sizeof(inp), stdin) == NULL) {
        // Handle error or exit gracefully
        fprintf(stderr, "Error reading input.\n");
        exit(EXIT_FAILURE);
        break;
        }
        if(strcmp(inp, ".") != 0){
            break;
        }
        splitup = string_split(inp, sep, &num_words);
        // call function
        for(int i = 0; i < sizeof(splitup); i++){
        printf("%s \n", splitup[i]);
        }
    }
    for (int i = 0; i < num_words; i++)
    {
        free(splitup[i]);
    }
    free(splitup);
}
