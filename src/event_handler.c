#include "event_handler.h"
#include "error_handler.h"

event_handler_t handler;

void event_handler_init() {
    handler.queue_ready.head = 0;
    handler.queue_ready.tail = 0;
    handler.queue_pending.head = 0;
    handler.queue_pending.tail = 0;
    handler.current_time = 0;
}

void event_post(const event_t event) {
    if (event.eta == 0) {
        if (event_ring_full(&handler.queue_ready)) {
            sys_exit(-1, "Event queue (ready) is full");
        }
        event_ring_put(&handler.queue_ready, &event);
    } else {
        if (event_ring_full(&handler.queue_pending)) {
            sys_exit(-1, "Event queue (pending) is full");
        }
        event_ring_put(&handler.queue_pending, &event);
    }
}

event_t *event_pop() {
    if (event_ring_empty(&handler.queue_ready)) {
        return NULL;
    }
    return event_ring_get(&handler.queue_ready);
}
