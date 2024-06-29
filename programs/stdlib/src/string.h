#ifndef PEACHOS_STRING_H
#define PEACHOS_STRING_H

#include <stdbool.h>

char tolower(char s1);
int strlen(const char* ptr);
int strnlen(const char* ptr, int max);
int strnlen_terminator(const char* str, int max, char terminator);
int istrncmp(const char* s1, const char* s2, int n);
int strncmp(const char* str1, const char* str2, int n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, int count);
bool isdigit(char c);
int tonumericdigit(char c);
/** @brief function "strtok()" is used to split a string into tokens or smaller strings based on a designated delimiter */
char* strtok(char* str, const char* delimiters);

#endif