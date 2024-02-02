#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
        char *sep;
        if(argc < 2){
            sep = " \t";
        }
        else{
        sep = argv[1];
        }
        int *num_words;
        char inp[4000];
        char **splitup;
        while(strcmp(inp, ".") != 0){ 
        //read input, call string split to split it up, print out input,  until .
        fgets(inp, sizeof(inp), stdin);
        splitup = string_split(inp, sep, &num_words);
        //call function
        printf("%s", splitup);
        }
        for(int i = 0; i < num_words; i++){
        free(splitup[i]);
        }
        free(splitup);
        
    }
 
