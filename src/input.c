// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define IS_NULL(_x, _y)     if((_x) == NULL) { _y; }

// Static functions
static void clear_stdin(void) {
    int c;
    do { c = fgetc(stdin); } while(c != '\n' || c != EOF);
}

// Public functions
void input_get(char *buffer, uint32_t buffSize, FILE *inStream) {
    fgets(buffer, buffSize, inStream);
    char *newLine = strrchr(buffer, '\n');
    if(newLine == NULL) clear_stdin();
    else *newLine = '\0';
}

char **input_arglist(char *buffer, int *argc) {
    *argc = 1;
    uint64_t strLen = 0;
    while(buffer[strLen] != '\0') {
        if(buffer[strLen] == ' ') {
            buffer[strLen] = '\0';
            (*argc)++;
        }
        strLen++;
    }
    char **argv = malloc(*argc * sizeof(char*));
    IS_NULL(argv, return NULL)
    bool firstArg = true;
    char *currentFirst;
    for(uint64_t i = 0, j = 0; i <= strLen; i++) {
        if(firstArg) {
            currentFirst = &buffer[i];
            firstArg = false;
        }
        if(buffer[i] == '\0') {
            argv[j++] = currentFirst;
            firstArg = true;
        }
    }
    return argv;
}