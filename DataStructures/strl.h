#ifdef __linux__
#ifndef strl_h
#define strl_h

#include <aio.h>

size_t
strlcpy(char * __restrict dst, const char * __restrict src, size_t dsize);
size_t
strlcat(char *dst, const char *src, size_t siz);
char *
strnstr(const char *s, const char *find, size_t slen);
#endif
#endif