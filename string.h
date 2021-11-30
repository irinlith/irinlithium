#ifndef IRINLITHIUM_STRING_H
#define IRINLITHIUM_STRING_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#ifdef _WIN32
	#include <windows.h>
#elif __unix__
	#include <unistd.h>
#endif

void strconcat(char *dst, char *src, size_t n);
char *fgetstr(char *str, size_t *n, FILE *stream);
long ctol(char *str);
float ctof(char *str);
size_t strlength(const char *str);
long digits(long x);
long fdigits(float x);
bool strcontain(char *str, const char x);
bool strcompare(const char *str, const char *str2);
void strset(char *str, unsigned char value, size_t num);
bool strnumeric(const char *str);

#endif
