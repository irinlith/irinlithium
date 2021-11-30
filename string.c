#include "string.h"

void strconcat(char *dst, char *src, size_t n) {
	char *src_it = src;
	char *dst_it = dst + strlength(dst);
	for (; n && *src_it; dst_it++, src_it++, n--)
		*dst_it = *src_it;
	*dst_it = '\0';
}

char *fgetstr(char *str, size_t *n, FILE *stream) {
	if (!(str && stream && n)) return NULL;
	do {
		fgets(str, *n, stream);
	} while (*str == '\n');
	return str;
}

long ctol(char *str) {
	if (!str) return 0;
	register long x = 0;
	bool neg = (*str == '-' ? true : false);
	for (; *str; str++) {
		if (*str >= '0' && *str <= '9') {
			x *= 10;
			x += (*str - 48) * (neg ? -1 : 1);
		}
	}
	return x;
}

float ctof(char *str) {
	if (!str) return 0;
	register float x = 0, mult = 0.1;
	bool isSmall = false, neg = (*str == '-' ? true : false);
	for (; *str; str++) {
		if (*str == '.') isSmall = true;
		else if (*str >= '0' && *str <= '9') {
			if (!isSmall) x *= 10;
			x += (*str - 48) * (!isSmall ? 1 : mult) * (neg ? -1 : 1);
			if (isSmall) mult *= 0.1;
		}
	}
	return x;
}

size_t strlength(const char *str) {
	if (!str) return 0;
	const char *beg = str;
	for (; *str; ++str);
	return (size_t)(str - beg);
}

long digits(long x) {
	register long y = 1;
	for (; x > 9; y++, x /= 10);
	return y;
}

long fdigits(float x) {
	for (; x - (long)x; x *= 10);
	return digits((long)x);
}

bool strcontain(char *str, const char x) {
	if (!str) return 0;
	for (; *str; str++) {
		if (*str == x) return 1;
	}
	return 0;
}

bool strcompare(const char *str, const char *str2) {
	if (!str || !str2) return 0;
	for (; *str && *str2; str++, str2++)
		if (*str != *str2) return 0;
	return (*str && !*str2) || (!*str && *str2) ? 0 : 1;
}

void strset(char *str, unsigned char value, size_t num) {
	for (; num; str++, num--) *str = value;
}


bool strnumeric(const char *str) {
	for (size_t i = 0; i < strlength(str); i++)
		if (!((str[i] >= '0' && str[i] <= '9') || str[0] == '-')) return 0;
	return 1;
}
