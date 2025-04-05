#ifndef PROCESS_H
#define PROCESS_H

#include <ring.h>
#include <stdint.h>

#define MAX_PROCESSES  10

/*
 * Process states
 */
typedef enum {
  CREATED,
  READY,
  RUNNING,
  TERMINATED
} process_state_t;

/*
 * Process context
 */
typedef struct p_context {
  uint32_t pid;
  process_state_t state;
  ring_t ring;
  void (*read_listener)(void);
  void (*write_listener)(void);
} p_context_t;

/*
 * Process structure
 */
typedef struct process {
  p_context_t context;
  void (*entry_point)(void* cookie);
  void *entry_point_cookie;
} process_t;

extern process_t process_table[MAX_PROCESSES];
extern uint32_t process_count;
extern process_t *active_process;

/*
 * Initialize a process
 */
uint32_t process_init(process_t *process, void (*entry_point)(void*),
                      void* cookie,
                      void (*read_listener)(void),
                      void (*write_listener)(void));

/*
 * Create a process
 */
process_t* process_create(void (*entry_point)(void*),
                          void* cookie,
                          void (*read_listener)(void),
                          void (*write_listener)(void));

/*
 * Start all processes
 */
void process_start_all();

/*
 * Start a specific process
 */
void process_start(process_t *process);

/*
 * Add a process to the process table
 */
void process_add(const process_t *process);

#endif //PROCESS_H
