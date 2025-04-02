#ifndef RING_H
#define RING_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_CHARS 512

/*
 * Check if the ring buffer is empty
 */
bool ring_empty();

/*
 * Check if the ring buffer is full
 */
bool ring_full();

/*
 * Add a byte to the ring buffer
 */
void ring_put(uint8_t bits);

/*
 * Get a byte from the ring buffer
 */
uint8_t ring_get();

#endif //RING_H
