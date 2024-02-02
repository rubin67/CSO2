{\rtf1\ansi\ansicpg1252\cocoartf2709
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red183\green111\blue179;\red23\green23\blue23;}
{\*\expandedcolortbl;;\cssrgb\c77255\c52549\c75294;\cssrgb\c11765\c11765\c11765;}
\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\partightenfactor0

\f0\fs24 \cf2 \cb3 \expnd0\expndtw0\kerning0
#include <stdio.h>\
#include <stdlib.h>\
#include <string.h>\
#include "split.h"\
\
char **string_split(const char *input, const char *sep, int *num_words) \{\
    // Initialize result array\
    char **result = NULL;\
\
    // Allocate initial space for the result array\
    result = (char **)calloc(1, sizeof(char *));\
    if (result == NULL) \{\
        // Handle memory allocation failure\
        fprintf(stderr, "Memory allocation error.\\n");\
        exit(EXIT_FAILURE);\
    \}\
\
    // Initialize variables\
    int input_len = strlen(input);\
    int sep_len = strlen(sep);\
    int start = 0;\
    int end = 0;\
\
    // Process each character in the input string\
    while (end <= input_len) \{\
        // Check if the current substring is a separator or the end of the string\
        if (strncmp(input + end, sep, sep_len) == 0 || input[end] == '\\0') \{\
            // Calculate the length of the word\
            int word_len = end - start;\
\
            // Resize the result array\
            result = (char **)realloc(result, (*num_words + 1) * sizeof(char *));\
            if (result == NULL) \{\
                // Handle memory allocation failure\
                fprintf(stderr, "Memory allocation error.\\n");\
                exit(EXIT_FAILURE);\
            \}\
\
            // Allocate space for the word and copy it\
            result[*num_words] = (char *)malloc(word_len + 1);\
            if (result[*num_words] == NULL) \{\
                // Handle memory allocation failure\
                fprintf(stderr, "Memory allocation error.\\n");\
                exit(EXIT_FAILURE);\
            \}\
\
            strncpy(result[*num_words], input + start, word_len);\
            result[*num_words][word_len] = '\\0';\
\
            // Move to the next word\
            (*num_words)++;\
            start = end + sep_len;\
\
            // Skip consecutive separators\
            while (input[start] != '\\0' && strncmp(input + start, sep, sep_len) == 0) \{\
                start += sep_len;\
            \}\
        \}\
\
        // Move to the next character in the input string\
        end++;\
    \}\
\
    return result;\
\}\
}