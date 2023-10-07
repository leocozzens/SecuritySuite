#ifndef INPUT_H
#define INPUT_H

typedef struct _IO_FILE FILE;

void input_get(char *buffer, int buffSize, FILE *inStream);

#endif