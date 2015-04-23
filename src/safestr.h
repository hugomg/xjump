/* These alternatives to the library functions error out uf the buffer is too small */

#include <stddef.h>

int try_snprintf(char *buf, size_t len, const char *fmt, ...);
char * try_strncpy(char *dest, const char *src, size_t n);