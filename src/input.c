// C stdlib headers
#include <stdio.h>
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