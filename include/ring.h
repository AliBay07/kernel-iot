#ifndef RING_H
#define RING_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_RING_SIZE 256

typedef struct ring {
    volatile uint8_t buffer[MAX_RING_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
} ring_t;

/*
 * Check if the ring buffer is empty
 */
static __inline__ bool ring_empty(const ring_t* ring) {
    return ring->head == ring->tail;
}

/*
 * Check if the ring buffer is full
 */
static __inline__ bool ring_full(const ring_t* ring) {
    return (ring->head + 1) % MAX_RING_SIZE == ring->tail;
}

/*
 * Put a byte into the ring buffer
 */
static __inline__ void ring_put(ring_t* ring, const uint8_t byte) {
    const uint32_t next = (ring->head + 1) % MAX_RING_SIZE;
    ring->buffer[ring->head] = byte;
    ring->head = next;
}

/*
 * Get a byte from the ring buffer
 */
static __inline__ uint8_t ring_get(ring_t* ring) {
    const uint8_t byte = ring->buffer[ring->tail];
    ring->tail = (ring->tail + 1) % MAX_RING_SIZE;
    return byte;
}

#endif //RING_H
