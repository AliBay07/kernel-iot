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

uint32_t strlen(const char *s) {
    uint32_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}
