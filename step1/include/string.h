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
int strlen(const char *s);

/* Minimal implementation of strcpy.
 * Copies the string pointed to by src to dest.
 * Returns a pointer to dest.
 */
char *strcpy(char *dest, const char *src);

/* Minimal implementation of memcpy.
 * Copies n bytes from src to dest.
 * Returns a pointer to dest.
 */
void *memcpy(void *dest, const void *src, uint32_t n);

#endif //STRING_H
