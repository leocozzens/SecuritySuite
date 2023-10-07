// C standard library
#include <stdio.h>
#include <string.h>

// Static functions
static void clear_stdin(void) {
    int c;
    do { c = fgetc(stdin); } while(c != '\n' || c != EOF);
}

// Public functions
void input_get(char *buffer, int buffSize, FILE *inStream) {
    fgets(buffer, buffSize, inStream);
    char *newLine = strrchr(buffer, '\n');
    if(newLine == NULL) clear_stdin();
    else *newLine = '\0';
}