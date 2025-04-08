#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <stdint.h>
#include "event_ring.h"
#include "event.h"

typedef struct event_handler {
    event_ring_t queue_ready;
    event_ring_t queue_pending;
    uint32_t current_time;
} event_handler_t;

/*
 * Initialize the event handler
 */
void event_handler_init();

/*
 * Add an event to the event handler
 */
void event_post(event_t event);

/*
 * Remove an event from the event handler, returns null if no event is available
 */
event_t* event_pop();


#endif //EVENT_HANDLER_H
