#include "allocator.h"
#include "string.h"

extern uint32_t __heap_start__;
extern uint32_t __heap_end__;

typedef struct block {
    uint32_t size;
    struct block *next;
} block_t;

static block_t *free_list = 0;

void init_heap(void) {
    free_list = (block_t *)&__heap_start__;
    free_list->size = (uint8_t *)&__heap_end__ - (uint8_t *)&__heap_start__;
    free_list->next = 0;
}

void* h_alloc(uint32_t size) {
    // Align requested size to 4 bytes
    size = (size + 3) & ~3;
    block_t *prev = 0;
    block_t *curr = free_list;

    while (curr) {

        // Check if the block is large enough to split off a block of the requested size
        // plus the header for the new free block
      if (curr->size >= size + sizeof(block_t)) {
          // Calculate remaining size after allocation
            const uint32_t remaining_size = curr->size - size - sizeof(block_t);
            void *allocated = (uint8_t *)curr + sizeof(block_t);
            if (remaining_size > sizeof(block_t)) {
                // Create a new block for the remaining space
                block_t *new_block = (block_t *)((uint8_t *)allocated + size);
                new_block->size = remaining_size;
                new_block->next = curr->next;
                if (prev) {
                    prev->next = new_block;
                } else {
                    free_list = new_block;
                }
            } else {
                // Not enough space to create a new block; allocate the entire block.
                allocated = (void *)((uint8_t *)curr + sizeof(block_t));
                if (prev) {
                    prev->next = curr->next;
                } else {
                    free_list = curr->next;
                }
            }
            return allocated;
        }
        prev = curr;
        curr = curr->next;
    }

    // No suitable block found
    return 0;
}

void h_free(void *ptr) {
    if (!ptr) {
        return;
    }

    // Get the block header corresponding to this allocation
    block_t *block_ptr = (block_t *)((uint8_t *)ptr - sizeof(block_t));
    block_t *prev = 0;
    block_t *curr = free_list;

    // Insert block_ptr into the free list in sorted order
    while (curr && curr < block_ptr) {
        prev = curr;
        curr = curr->next;
    }

    // Coalesce with previous block if adjacent
    if (prev && (uint8_t *)prev + sizeof(block_t) + prev->size == (uint8_t *)block_ptr) {
        prev->size += sizeof(block_t) + block_ptr->size;
        block_ptr = prev;
    } else {
        block_ptr->next = curr;
        if (prev) {
            prev->next = block_ptr;
        } else {
            free_list = block_ptr;
        }
    }

    // Coalesce with next block if adjacent
    if (curr && ((uint8_t *)block_ptr + sizeof(block_t) + block_ptr->size == (uint8_t *)curr)) {
        block_ptr->size += sizeof(block_t) + curr->size;
        block_ptr->next = curr->next;
    }
}
