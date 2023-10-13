// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    uint64_t buffLen = strlen(buffer);
    for(uint64_t i = 0; i < buffLen; i++)
        if(buffer[i] == ' ') {
            buffer[i] = '\0';
            (*argc)++;
        }
    char **argv = malloc(*argc * sizeof(char*));
    argv[0] = buffer;

    uint64_t j = 1;
    for(uint64_t i = 0; i < buffLen; i++)
        if(buffer[i] == '\0') argv[j++] = buffer + i + 1;
    return argv;
}