#ifndef EVENT_RING_H_
#define EVENT_RING_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "event.h"

#define EVENT_RING_MAX 32

typedef struct event_ring {
    event_t events[EVENT_RING_MAX];
    volatile uint32_t head;
    volatile uint32_t tail;
} event_ring_t;

static __inline__ bool event_ring_empty(const event_ring_t* ring) {
    return ring->head == ring->tail;
}

static __inline__ bool event_ring_full(const event_ring_t* ring) {
    return ((ring->head + 1) % EVENT_RING_MAX) == ring->tail;
}

static __inline__ void event_ring_put(event_ring_t* ring, const event_t* event) {
    const uint32_t next = (ring->head + 1) % EVENT_RING_MAX;
    ring->events[ring->head] = *event;
    ring->head = next;
}

static __inline__ event_t* event_ring_get(event_ring_t* ring) {
    if (event_ring_empty(ring))
        return NULL;
    event_t* ret = &ring->events[ring->tail];
    ring->tail = (ring->tail + 1) % EVENT_RING_MAX;
    return ret;
}

#endif // EVENT_RING_H_