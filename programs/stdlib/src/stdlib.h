#ifndef MAEROS_STDLIB_H
#define MAEROS_STDLIB_H
#include <stddef.h>

/** @brief user space memory allocation */
void* malloc(size_t size);

/** @brief user memory free */
void free(void* ptr);

/** @brief return an integer number as char */
char* itoa(int i);
#endif