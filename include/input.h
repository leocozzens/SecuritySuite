#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

typedef FILE _FILE;

bool input_init(uint64_t buffInitSize, uint32_t growthFactor, char **errVal);
char *input_get(_FILE *inStream, char **errVal);
char **input_arglist(const char *buffer, int *argc);
void input_cleanup(void);

#endif