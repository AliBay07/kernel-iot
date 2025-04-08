#ifndef TASK_H
#define TASK_H

#include <ring.h>
#include <stdint.h>

#define MAX_TASKS 10

/*
 * Task context
 */
typedef struct t_context {
    uint32_t pid;
    ring_t ring;
    void (*read_listener)(void *);
    void (*write_listener)(void *);
    void *read_cookie;
    void *write_cookie;
} t_context_t;

/*
 * Process structure
 */
typedef struct task {
    t_context_t context;
    void (*init)(t_context_t*, void*);
    void *init_cookie;
} task_t;


/*
 * Create a task
 */
task_t *task_create(void (*init)(t_context_t*, void *),
                    void *cookie,
                    void (*read_listener)(void *),
                    void (*write_listener)(void *),
                    uint32_t uartno);

/*
 * Activates a task
 */
void task_activate(task_t *task);

#endif //TASK_H
