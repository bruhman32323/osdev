#ifndef _UTIL_H
#define _UTIL_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOW_16(address) (uint16_t)((address) & 0xFFFF)            
#define HIGH_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

void* malloc(size_t amount);

int rand(void);
void srand(size_t seed);
char* itoa(int res);

#ifdef __cplusplus
}
#endif

#endif /* _UTIL_H */