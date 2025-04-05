/*
 * Copyright: Olivier Gruber (olivier dot gruber at acm dot org)
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include "main.h"
#include "isr.h"
#include "process.h"
#include "uart.h"
#include "shell.h"
#include "allocator.h"
#include "error_handler.h"

extern uint32_t irq_stack_top;
extern uint32_t stack_top;
extern uint32_t __heap_start__;
extern uint32_t __heap_end__;

void check_stacks() {
    const void *memsize = (void *) MEMORY;
    const void *addr = &stack_top;

    if (addr >= memsize)
        sys_exit(-1,"Stack overflow");

    addr = &irq_stack_top;
    if (addr >= memsize)
        sys_exit(-1,"IRQ stack overflow");

    addr = &__heap_start__;
    if (addr >= memsize)
        sys_exit(-1,"Heap start overflow");

    addr = &__heap_end__;
    if (addr >= memsize)
        sys_exit(-1,"Heap end overflow");
}

/**
 * Initialize the system
 */
void sys_init() {
    init_heap();
    setup_uarts();
    setup_irqs();
    check_stacks();
}

/**
 * This is the C entry point,
 * upcalled once the hardware has been setup properly
 * in assembly language, see the startup.s file.
 */
void _start(void) {
    sys_init();
    process_t* p_shell = process_create(shell_start, NULL,
        shell_read_listener, NULL);
    process_start(p_shell);
    sys_exit(0,"End of _start entry point");
}
