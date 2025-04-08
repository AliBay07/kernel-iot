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

#include "uart.h"
#include <error_handler.h>
#include "main.h"
#include "uart-mmio.h"
#include "ring.h"
#include "task.h"
#include <stdint.h>
#include "event_handler.h"

typedef struct uart {
    uint8_t uartno; // the UART numéro
    void *bar; // base address register for this UART
    t_context_t* t_context; // pointer to the task context
} uart_t ;

uart_t uarts[NUARTS];

static void uart_init(const uint32_t uartno, void *bar) {
    uart_t *uart = &uarts[uartno];
    uart->uartno = uartno;
    uart->bar = bar;
    uart->t_context = NULL;

    mmio_write32(uart->bar, UART_IMSC, 0x0000);
}

void uarts_init() {
    uart_init(UART0, UART0_BASE_ADDRESS);
    uart_init(UART1, UART1_BASE_ADDRESS);
    uart_init(UART2, UART2_BASE_ADDRESS);
}

void uart_grab(const uint32_t uartno, t_context_t* t_context) {
    uart_t *uart = &uarts[uartno];
    uart->t_context = t_context;
}

/*
 * Setup the UARTs
 */
void setup_uarts() {
    uarts_init();
    uart_enable(UART0);
    uart_send_string(UART0, "UART's setup has been completed...\n");
}

void uart_enable(const uint32_t uartno) {
    const uart_t *uart = &uarts[uartno];
    mmio_write32(uart->bar, UART_IMSC, UART_IMSC_RXIM);
}

void uart_disable(const uint32_t uartno) {
    const uart_t *uart = &uarts[uartno];
    mmio_write32(uart->bar, UART_IMSC, 0);
}

void uart_interrupt(void *cookie_uart) {
    const cookie_uart_t *cookie = (cookie_uart_t *) cookie_uart;
    const uart_t *uart = &uarts[cookie->uartno];

    if (uart->t_context == NULL) {
        sys_exit(-1, "UART not linked to any task !");
    };

    char c;
    uart_receive(cookie->uartno, &c);
    while (c) {
        if (ring_full(&uart->t_context->ring)) sys_exit(-1, "Ring full in UART interrupt");
        ring_put(&uart->t_context->ring, c);
        uart_receive(cookie->uartno, &c);
    }

    event_t event;
    event.callback = uart->t_context->read_listener;
    event.cookie = uart->t_context->read_cookie;
    event.eta = 0;
    event_post(event);
}

void uart_receive(const uint8_t uartno, char *pt) {
    const uart_t *uart = &uarts[uartno];
    if (mmio_read8(uart->bar, UART_FR) & UART_FR_RXFE) {
        *pt = '\0';
        return;
    }
    *pt = mmio_read8(uart->bar, UART_DR);
}

/**
 * Sends a character through the given uart, this is a blocking call
 * until the character has been sent.
 */
void uart_send(const uint8_t uartno, const char s) {
    const uart_t *uart = &uarts[uartno];
    while (mmio_read8(uart->bar, UART_FR) & UART_FR_TXFF) {}
    mmio_write8(uart->bar, UART_DR, s);
}

/**
 * This is a wrapper function, provided for simplicity,
 * it sends a C string through the given uart.
 */
void uart_send_string(const uint8_t uartno, const char *s) {
    while (*s != '\0') {
        uart_send(uartno, *s);
        s++;
    }
}
