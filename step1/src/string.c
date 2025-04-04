#include "string.h"

int strncmp(const char *s1, const char *s2, uint32_t n) {
    uint32_t i;
    for (i = 0; i < n; i++) {
        if (s1[i] != s2[i]) {
            return ((unsigned char)s1[i]) - ((unsigned char)s2[i]);
        }
        if (s1[i] == '\0') {
            break;
        }
    }
    return 0;
}

int strlen(const char *s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while ((*dest++ = *src++) != '\0');
    return ret;
}

void *memcpy(void *dest, const void *src, uint32_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}


