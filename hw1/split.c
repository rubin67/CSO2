#include "split.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
char **string_split(const char *input, const char *sep, int *num_words)
{

   // nested for loop that goes checks input until it reaches null terminator
   char **array = (char **)malloc(sizeof(char *));
   // iterator for input
   int i = 0;

   // size of word and seperator
   int wordLen = 0;
   // index of array
   *num_words = 0;
   if (strcspn(input, sep) == 0)
   {
      // size of the a
      array[*num_words] = (char *)malloc(sizeof(char) * 1);
      strcpy(array[0], "");
      (*num_words)++;
      i += strspn(input, sep);
   }

   while (input[i] != '\0')
   {
      wordLen = strcspn(input + i, sep);
      array = (char **)realloc(array, sizeof(char *) * (*num_words+1));
      array[*num_words] = (char *)malloc(sizeof(char) * (wordLen + 1));
      strncpy(array[(*num_words)], input + i, wordLen);
      array[(*num_words)][wordLen] = '\0';
      i += wordLen;
      (*num_words)++;

      wordLen = strspn(input+i, sep);
      i += wordLen;

      if (input[i] == '\0')
      {
         array = (char **)realloc(array, sizeof(char *) * (*num_words+1));
         array[*num_words] = (char *)malloc(sizeof(char) * (wordLen + 1));
         strcpy(array[(*num_words)], "");
         (*num_words)++;
      }
   }
   return array;
}
