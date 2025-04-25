#ifndef SYSMEM_H
#define SYSMEM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limine.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

#endif // MEMORY_H