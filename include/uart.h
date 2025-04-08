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

#ifndef UART_H_
#define UART_H_

/*
 * Defines the number of available UARTs
 * and their respective numéro.
 */
#include <task.h>
#include <stdint.h>
#define NUARTS 3
#define UART0 0
#define UART1 1
#define UART2 2

/*
 * Structure to pass the uart cookie to the interrupt handler
 */
typedef struct cookie_uart {
    uint8_t uartno;
} cookie_uart_t;

/*
 * This is the interrupt handler for the UARTs.
 * It is called by the ISR, and it is responsible
 * for reading the character from the UART RX FIFO
 * queue and .
 */
void uart_interrupt(void* cookie_uart);

/*
 * Receives a one-byte character, which is compatible
 * with ASCII encoding. This function blocks, spinning,
 * until there is one character available, that is,
 * there is at least one character available in the
 * UART RX FIFO queue.
 */
void uart_receive(uint8_t uartno, char *pt);

/**
 * Write a one-byte character through the given uart,
 * this is a blocking call. This is compatible with a
 * basic ASCII encoding. This function blocks, spinning,
 * until there is room in the UART TX FIFO queue to send
 * the character.
 */
void uart_send(uint8_t uartno, char s);

/**
 * This is a wrapper function, provided for simplicity,
 * it sends the given C string through the given uart,
 * using the function uart_send.
 */
void uart_send_string(uint8_t uartno, const char *s);


/*
 * Enables the UARTs
 */
void setup_uarts();


/*
 * Global initialization for all the UARTs
 */
void uarts_init();

/*
 * This function is called by the UART interrupt handler
 * to link the UART to the process context.
 * The process context is used to send the received
 * characters to the process.
 */
void uart_grab(uint32_t uartno, t_context_t* t_context);

/*
 * Enables the UART, identified by the given numéro.
 * Nothing to do on QEMU until we use interrupts...
 * You can enable or disable the individual interrupts by changing
 */
void uart_enable(uint32_t uartno);

/*
 * Disables the UART, identified by the given numéro.
 * Nothing to do on QEMU until we use interrupts...
 */
void uart_disable(uint32_t uartno);

#endif /* UART_H_ */
