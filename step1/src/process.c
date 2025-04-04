#include "process.h"
#include "allocator.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

unsigned int process_count = 0;

process_t process_table[MAX_PROCESSES];

uint32_t process_init(process_t *process, void (*entry_point)(void)) {
  if (process == NULL || entry_point == NULL) {
    return 0;
  }
  process->pid = process_count;
  process->state = CREATED;
  process->entry_point = entry_point;
  process_add(process);
  return process->pid;
}

process_t* process_create(void (*entry_point)(void)) {
  process_t *process = h_alloc(sizeof(process_t));
  process_init(process, entry_point);
  return process;
}

void process_add(const process_t *process) {
  if (process_count < MAX_PROCESSES) {
    process_table[process_count++] = *process;
  }
}

void process_start_all() {
  for (unsigned int i = 0; i < process_count; i++) {
    process_t *process = &process_table[i];
    if (process->state == CREATED) {
      process->state = READY;
      process_start(process);
    }
  }
}

void process_start(process_t *process) {
  process->state = RUNNING;
  process->entry_point();
  process->state = TERMINATED;
  h_free(process);
}
