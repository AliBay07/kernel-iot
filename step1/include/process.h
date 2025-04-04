#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PROCESSES 10

/*
 * Process states
 */
typedef enum {
  CREATED,
  RUNNING,
  READY,
  BLOCKED,
  WAITING,
  TERMINATED
} process_state_t;

/*
 * Process structure
 */
typedef struct process {
  uint32_t pid;
  process_state_t state;
  void (*entry_point)(void);
} process_t;

/*
 * Initialize a process
 */
uint32_t process_init(process_t *process, void (*entry_point)(void));

/*
 * Create a process
 */
process_t* process_create(void (*entry_point)(void));

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
