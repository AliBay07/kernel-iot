#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdint.h>

/*
 * Initialises the heap
 */
void init_heap(void);

/*
 * Allocates a block of memory of the specified size on the heap
 */
void *h_alloc(uint32_t size);

/*
 * Free's a block of memory from the heap
 */
void h_free(void *ptr);

#endif //ALLOCATOR_H
