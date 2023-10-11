#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

typedef FILE FILE_DEF;

void input_get(char *buffer, uint32_t buffSize, FILE *inStream);

#endif