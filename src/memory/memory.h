#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

/**
 * @brief memory set function, fill memory with a value
*/
void* memset(void* ptr, int c, size_t size);

/**
 * @brief compare memory regions by the amount of 'count' bytes
*/
int memcmp(void* s1, void* s2, int count);

/**
 * @brief copy data from 'dest' to 'src' address by the amount of 'len' bytes
*/
void* memcpy(void* dest, void* src, int len);

#endif