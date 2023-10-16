// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
// Local headers
#include <errors.h>
#include <operations.h>
#include <strchrnul.h>

#define IS_NULL(_x, _y)     if((_x) == NULL) { _y; }

typedef struct {
    char *data;
    uint64_t buffSize;
    uint32_t growthFactor;
} BuffData;

static BuffData buffInfo;
bool input_init(uint64_t buffInitSize, uint32_t growthFactor, char **errVal) {
    buffInfo.data = malloc(buffInitSize);
    IS_NULL(buffInfo.data, *errVal = MEM_ERR; return true)
    buffInfo.buffSize = buffInitSize;
    buffInfo.growthFactor = growthFactor;
    return false;
}

char *input_get(FILE *inStream, char **errVal) {
    uint64_t offSet = 0;
    uint64_t readSize = buffInfo.buffSize;
    bool sizeErr = false;
    char *newLine;
    do {
        IS_NULL(fgets(buffInfo.data + offSet, readSize, inStream), *errVal = INPUT_STREAM_ERR; DONE(0, NULL); return NULL)
        newLine = local_strchrnul(buffInfo.data + offSet, '\n');
        if(*newLine == '\0') {
            offSet = newLine - buffInfo.data;
            sizeErr = true;
        }
        else sizeErr = false;

        if(sizeErr) {
            char *tmp = realloc(buffInfo.data, buffInfo.buffSize * buffInfo.growthFactor);
            IS_NULL(tmp, *errVal = INPUT_MEM_ERR;  return NULL)
            buffInfo.data = tmp;
            readSize = (buffInfo.buffSize * buffInfo.growthFactor) - buffInfo.buffSize;
            buffInfo.buffSize *= buffInfo.growthFactor;
        }
    } while(sizeErr);
    *newLine = '\0';
    return buffInfo.data;
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
    for(int i = 0; i < *argc; i++) {
        printf("i#%d - %s\n", i, argv[i]);
    }
    return argv;
}