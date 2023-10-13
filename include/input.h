#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

typedef FILE _FILE;

void input_get(char *buffer, uint32_t buffSize, _FILE *inStream);

#endif