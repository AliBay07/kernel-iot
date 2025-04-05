#include "process.h"
#include "allocator.h"
#include <stdint.h>
#include <stdlib.h>

process_t process_table[MAX_PROCESSES];
uint32_t process_count = 0;
process_t *active_process = 0;

uint32_t process_init(process_t *process, void (*entry_point)(void*),
                      void* cookie,
                      void (*read_listener)(void),
                      void (*write_listener)(void)) {
  if (process == NULL || entry_point == NULL) {
    return 0;
  }
  process->context.pid = process_count;
  process->context.state = CREATED;
  process->context.ring.head = 0;
  process->context.ring.tail = 0;

  for (int i = 0; i < MAX_RING_SIZE; i++) {
    process->context.ring.buffer[i] = '\0';
  }

  process->context.read_listener = read_listener;
  process->context.write_listener = write_listener;
  process->entry_point = (void*)entry_point;
  process->entry_point_cookie = cookie;
  process_add(process);
  return process->context.pid;
}

process_t* process_create(void (*entry_point)(void*),
                          void* cookie,
                          void (*read_listener)(void),
                          void (*write_listener)(void)) {
  process_t *process = h_alloc(sizeof(process_t));
  if (process) {
    process_init(process, entry_point, cookie,
      read_listener, write_listener);
  }
  return process;
}

void process_add(const process_t *process) {
  if (process_count < MAX_PROCESSES) {
    process_table[process_count++] = *process;
  }
}

void process_remove(const process_t *process) {
  for (unsigned int i = 0; i < process_count; i++) {
    if (process_table[i].context.pid == process->context.pid) {
      for (unsigned int j = i; j < process_count - 1; j++) {
        process_table[j] = process_table[j + 1];
      }
      process_count--;
      break;
    }
  }
}

void process_start_all() {
  for (unsigned int i = 0; i < process_count; i++) {
    process_t *process = &process_table[i];
    if (process->context.state == CREATED) {
      process->context.state = READY;
      process_start(process);
    }
  }
}

void process_start(process_t *process) {
  process->context.state = RUNNING;
  active_process = process;
  process->entry_point(process->entry_point_cookie);
  process->context.state = TERMINATED;
  active_process = 0;
  process_remove(process);
  h_free(process);
}
