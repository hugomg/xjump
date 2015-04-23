#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int try_snprintf(char *buf, size_t len, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int n = vsnprintf(buf, len, fmt, args);
  if (n >= len){
    fprintf(stderr, "fatal error: filename is too long\n");
    exit(1);
  }
  va_end(args);
  return n;
}

char * try_strncpy(char *dest, const char *src, size_t n)
{
  if(strlen(src) >= n){
    fprintf(stderr, "fatal error: filename is too long\n");
    exit(1);
  }
  return strncpy(dest, src, n);
}