#include "shell.h"
#include "uart.h"
#include <stdint.h>
#include "string.h"

#define SHELL_BUFFER_SIZE 256

char shell_buffer[SHELL_BUFFER_SIZE];
uint32_t shell_index = 0;

void shell_process_command(const char *cmd) {
    if (strncmp(cmd, "echo", 4) == 0) {
        const char *arg = cmd + 4;
        while (*arg == ' ') {
            arg++;
        }
        uart_send_string(UART0, arg);
        uart_send_string(UART0, "\r\n");
    } else if (strncmp(cmd, "clear", 5) == 0) {
        uart_send_string(UART0, "\033[2J\033[H");
    } else if (strncmp(cmd, "curs", 4) == 0) {
        const char *arg = cmd + 4;
        while (*arg == ' ') {
            arg++;
        }
        if (strncmp(arg, "on", 2) == 0) {
            uart_send_string(UART0, "\033[?25h");
        } else if (strncmp(arg, "off", 3) == 0) {
            uart_send_string(UART0, "\033[?25l");
        } else {
            uart_send_string(UART0, "Invalid argument for curs command\r\n");
        }
    }
    else if (strncmp(cmd, "help", 4) == 0) {
        uart_send_string(UART0, "Available commands:\r\n");
        uart_send_string(UART0, "echo <message> - Echo the message back\r\n");
        uart_send_string(UART0, "clear - Clear the screen\r\n");
        uart_send_string(UART0, "curs <on|off> - Turn cursor on or off\r\n");
        uart_send_string(UART0, "help - Show this help message\r\n");
    } else {
        if (cmd[0] != '\0') {
            uart_send_string(UART0, "Unknown command: ");
            uart_send_string(UART0, cmd);
            uart_send_string(UART0, "\r\n");
        }
    }

    uart_send_string(UART0, "> ");
}

void shell_process_char(char c) {

 if (c == 0x7F || c == '\b') {
        if (shell_index > 0) {
            shell_index--;
            uart_send_string(UART0, "\b \b");
        }
    } else if (c == '\r' || c == '\n') {
        shell_buffer[shell_index] = '\0';
        uart_send_string(UART0, "\r\n");
        shell_process_command(shell_buffer);
        shell_index = 0;
        shell_buffer[0] = '\0';
    } else if (c >= 0x20 && c <= 0x7E) {
        if (shell_index < SHELL_BUFFER_SIZE - 1) {
            shell_buffer[shell_index++] = c;
            shell_buffer[shell_index] = '\0';
            uart_send(UART0, c);
        }
    }
}

void shell_init() {
    shell_index = 0;
    shell_buffer[0] = '\0';
    uart_send_string(UART0, "> ");
}

