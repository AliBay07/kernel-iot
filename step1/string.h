#ifndef STRING_H
#define STRING_H

#include <stdint.h>

/* Minimal implementation of strncmp.
 * Compares up to n characters of s1 and s2.
 * Returns 0 if they are equal.
 */
int strncmp(const char *s1, const char *s2, uint32_t n);


/* Minimal implementation of strlen.
 * Returns the length of the null-terminated string.
 */
uint32_t strlen(const char *s);

#endif //STRING_H
