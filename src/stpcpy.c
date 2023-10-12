#define NULL    ((void*)0)

char *local_stpcpy(char *restrict dest, const char *restrict source) {
    if(dest == NULL || source == NULL) return NULL;
    while(*source != '\0') *dest++ = *source++;

    *dest = '\0';
    return dest;
}