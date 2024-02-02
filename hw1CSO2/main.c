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
#define MAX_INPUT_SIZE 4000\
\
int main(int argc, char *argv[]) \{\
    char *sep;\
\
    // Set sep based on command-line arguments\
    if (argc == 1) \{\
        sep = strdup(" \\t");\
    \} else \{\
        // Concatenate command-line arguments to form sep\
        int total_length = 0;\
        for (int i = 1; i < argc; ++i) \{\
            total_length += strlen(argv[i]);\
        \}\
\
        sep = (char *)malloc(total_length + argc - 1); // +1 for null terminator\
        sep[0] = '\\0';\
\
        for (int i = 1; i < argc; ++i) \{\
            strcat(sep, argv[i]);\
            if (i < argc - 1) \{\
                strcat(sep, " ");\
            \}\
        \}\
    \}\
\
    // Read lines of input\
    char input[MAX_INPUT_SIZE];\
    while (1) \{\
        printf("> ");\
        fgets(input, sizeof(input), stdin);\
\
        // Check for exit condition\
        if (input[0] == '.' && (input[1] == '\\n' || input[1] == '\\0')) \{\
            break;\
        \}\
\
        // Remove trailing newline\
        input[strcspn(input, "\\n")] = '\\0';\
\
        // Call string_split\
        int num_words;\
        char **result = string_split(input, sep, &num_words);\
\
        // Print the resulting array\
        for (int i = 0; i < num_words; ++i) \{\
            printf("[%s]", result[i]);\
        \}\
        printf("\\n");\
\
        // Free the resulting array\
        for (int i = 0; i < num_words; ++i) \{\
            free(result[i]);\
        \}\
        free(result);\
    \}\
\
    // Free sep\
    free(sep);\
\
    return 0;\
\}\
}