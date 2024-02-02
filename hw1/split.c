#include "split.h"
#include <stdlib.h>
#include <string.h>
char **string_split(const char *input, const char *sep, int *num_words){

// nested for loop that goes checks input until it reaches null terminator
char **array = (char **)malloc(sizeof(char *));
//iterator for input
int i = 0;

//size of word and seperator
int wordLen = 0;
// index of array
   if(strspn(input, sep) == 0){
      // size of the a
      array[*num_words] = (char *)malloc(sizeof(char) * (*num_words+1));
      strcpy(array[0], "");
      num_words++;
      i += strcspn(input, sep);
      }

   while(input[i] != '\0'){
      wordLen = strspn(input + i, sep);
      array = (char **)realloc(array, sizeof(char *));
      array[wordLen] = (char *)malloc(sizeof(char)*(wordLen+1));
      *num_words++;
      strncpy(array[*num_words], input+i, wordLen);
      i += wordLen;
      
      wordLen = strcspn(input, sep);
      i += wordLen;

      if(input[i+wordLen] == '\0'){
      array = (char **)realloc(array, sizeof(char *));
      array[wordLen] = (char *)malloc(sizeof(char)*(wordLen+1));
      strcpy(array[*num_words], "");
      }
   }
      return array;
 }


