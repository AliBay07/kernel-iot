#include "task.h"
#include "allocator.h"
#include "uart.h"
#include <stdint.h>
#include <stdlib.h>

uint32_t task_count = 0;

uint32_t task_init(task_t *task, void (*init)(t_context_t*, void*),
                      void* cookie,
                      void (*read_listener)(void*),
                      void (*write_listener)(void*),
                      const uint32_t uartno) {

  if (task == NULL || init == NULL) {
    return 0;
  }
  task->context.pid = task_count;
  task->context.ring.head = 0;
  task->context.ring.tail = 0;

  for (int i = 0; i < MAX_RING_SIZE; i++) {
    task->context.ring.buffer[i] = '\0';
  }

  task->context.read_listener = read_listener;
  task->context.write_listener = write_listener;
  task->init = (void*)init;
  task->init_cookie = cookie;

  uart_grab(uartno, &task->context);

  return task->context.pid;
}

task_t* task_create(void (*init)(t_context_t*, void*),
                          void* cookie,
                          void (*read_listener)(void*),
                          void (*write_listener)(void*),
                          const uint32_t uartno) {
  task_t *task = h_alloc(sizeof(task_t));
  if (task) {
    task_init(task, init, cookie,
      read_listener, write_listener, uartno);
  }
  return task;
}

void task_activate(task_t *task) {
  if (task == NULL) {
    return;
  }
  task->init(&task->context, task->init_cookie);
}
