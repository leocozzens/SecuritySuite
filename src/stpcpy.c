// C stdlib headers
#include <string.h>

char *local_stpcpy(char *dest, const char *src) {
	const size_t length = strlen(src);
    memcpy(dest, src, length + 1);
    return dest + length;
}