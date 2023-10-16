#define NULL ((void*)0)

char *local_strchrnul(char *input, char target) {
    if(input == NULL) return NULL;
    while(*input != target && *input != '\0') input++;
    return input;
}