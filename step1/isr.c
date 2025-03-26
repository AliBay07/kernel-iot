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

#include "isr.h"
#include "uart.h"
#include "isr-mmio.h"
#include "main.h"

/*
 * Assembly functions:
 */
extern void _irqs_setup(void);

extern void _irqs_enable(void);

extern void _irqs_disable(void);

extern void _wfi(void);

handler_t handlers[NIRQS];

/*
 * Interrupt Service Routine, up-called from assembly.
 * Needs to interogate the VIC about the masked interrupt
 * status and call the corresponding handlers.
 */
void isr() {
    core_disable_irqs();
    const uint32_t irqs = vic_load_irqs();
    for (uint32_t i = 0; i < NIRQS; i++) {
        const handler_t *handler = &handlers[i];
        if (irqs & (1 << i))
            handler->callback(handler->cookie);
    }
    core_enable_irqs();
    // vic_ack_irqs(irqs);
}

void core_enable_irqs() { _irqs_enable(); }

void core_disable_irqs() { _irqs_disable(); }

void core_halt() { _wfi(); }

/**
 * Enable the UART IRQs
 */
void vic_enable_uart_irqs(char* c) {
    cookie_uart_t cookie = {UART0, c};
    vic_enable_irq(UART0_IRQ, uart_interrupt, &cookie);
}

/**
 * Enable the system IRQs
 */
void vic_enable_irqs(char *c) {
   vic_enable_uart_irqs(c);
}

/**
 * Setup the IRQs
 */
void setup_irqs(const irqs_params* params) {
    vic_setup_irqs();
    vic_enable_irqs(params->uart0_char);
    uart_send_string(UART0, "IRQ's setup has been completed...\n");
}

/*
 * Initial setup our interrupt support,
 * need to initialize both the hardware and software
 * sides.
 */
void vic_setup_irqs() {
    for (uint32_t i = 0; i < NIRQS; i++) {
       vic_disable_irq(i);
    }
    _irqs_setup();
}

/*
 * Returns the list of interrupts
 */
uint32_t vic_load_irqs(void) {
    return mmio_read32((void *) VIC_BASE_ADDR, VICIRQSTATUS);
}

/*
 * Enables the given interrupt at the VIC level.
*/
void vic_enable_irq(const uint32_t irq, void (*callback)(void *),
                    void *cookie) {
    const handler_t handler = {callback, cookie};
    handlers[irq] = handler;
    mmio_write32((void *) VIC_BASE_ADDR, VICINTENABLE, 1 << irq);
}

/*
 * Disables the given interrupt at the VIC level.
 */
void vic_disable_irq(const uint32_t irq) {
    handlers[irq] = (handler_t){0, 0};
    mmio_write32((void *) VIC_BASE_ADDR, VICINTENABLE, 0 << irq);
}
