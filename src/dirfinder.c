// C stdlib headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <string.h>
#include <errno.h>
#endif
// Local headers
#include <errors.h>

#define PATH_MIN                256
#define GROWTH_FACTOR           2
#define S_LEN(_string)			(sizeof(_string) - 1)
#define IS_NULL(_x, _y)			if((_x) == NULL) { _y; }

char *dir_get_exec(const char **errVal) {
    size_t buffSize = PATH_MIN;
    ssize_t retVal = 0;
    char *execPath = malloc(buffSize);
    IS_NULL(execPath, *errVal = MEM_ERR; return NULL)
    bool sizeErr;
    do {
        sizeErr = false;
        retVal = readlink("/proc/self/exe", execPath, buffSize);
        if(execPath[buffSize - 1] != '\0') {
            sizeErr = true;
            buffSize *= GROWTH_FACTOR;
            char *tmp = realloc(execPath, buffSize);
            IS_NULL(tmp, free(execPath); *errVal = MEM_ERR; return NULL)
            execPath = tmp;
        }
    } while(sizeErr);
    if(retVal == -1) {
        *errVal = strerror(errno);
        free(execPath);
        return NULL;
    }
    char *lastSlash = strrchr(execPath, '/');
    size_t dirSize = (lastSlash == NULL) ? strlen(execPath) : lastSlash - execPath;
    dirSize++;  // This will include the '/' in new string
    char *execDir = malloc(dirSize + 1);
    strncpy(execDir, execPath, dirSize);
    free(execPath);
    return execDir;
}