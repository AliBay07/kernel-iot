#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

typedef struct event {
    void* cookie;
    void (*callback)(void*);
    uint32_t eta;
} event_t;

#endif //EVENT_H
