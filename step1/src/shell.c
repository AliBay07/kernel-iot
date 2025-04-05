#include "shell.h"
#include "uart.h"
#include "ring.h"
#include "isr.h"
#include "process.h"
#include <string.h>

#define SHELL_BUFFER_SIZE 256
#define HISTORY_SIZE 10

char shell_buffer[SHELL_BUFFER_SIZE];
int shell_index = 0;
int cursor_pos = 0;

char esc_buffer[3];
int esc_index = 0;

char command_history[HISTORY_SIZE][SHELL_BUFFER_SIZE];
int history_count = 0;
int history_index = -1;

int exit_shell = 0;

void shell_process_command(const char *cmd) {
    if (cmd[0] != '\0') {
        if (history_count < HISTORY_SIZE) {
            strcpy(command_history[history_count], cmd);
            history_count++;
        } else {
            for (int i = 0; i < HISTORY_SIZE - 1; i++) {
                strcpy(command_history[i], command_history[i + 1]);
            }
            strcpy(command_history[HISTORY_SIZE - 1], cmd);
        }
    }
    history_index = -1;

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
        while (*arg == ' ') arg++;
        if (strncmp(arg, "on", 2) == 0) {
            uart_send_string(UART0, "\033[?25h");
        } else if (strncmp(arg, "off", 3) == 0) {
            uart_send_string(UART0, "\033[?25l");
        } else {
            uart_send_string(UART0, "Invalid argument for curs command\r\n");
        }
    } else if (strncmp(cmd, "exit", 4) == 0) {
        exit_shell = 1;
        uart_send_string(UART0, "Exiting shell...\r\n");
        return;
    }
    else if (strncmp(cmd, "help", 4) == 0) {
        uart_send_string(UART0, "Available commands:\r\n");
        uart_send_string(UART0, "echo <message> - Echo the message back\r\n");
        uart_send_string(UART0, "clear - Clear the screen\r\n");
        uart_send_string(UART0, "curs <on|off> - Show or hide the cursor\r\n");
        uart_send_string(UART0, "exit - Exit the shell\r\n");
        uart_send_string(UART0, "help - Show this help message\r\n");
    } else {
        uart_send_string(UART0, "Unknown command: ");
        uart_send_string(UART0, cmd);
        uart_send_string(UART0, "\r\n");
    }
    uart_send_string(UART0, "> ");
}

void redraw_line(void) {
    uart_send_string(UART0, "\r> ");
    uart_send_string(UART0, shell_buffer);
    uart_send_string(UART0, "\033[K");
    for (int i = 0; i < shell_index - cursor_pos; i++) {
        uart_send_string(UART0, "\033[D");
    }
}

void shell_process_char(char c) {
    int i;
    // --- Handle escape sequences for arrow keys ---
    if (esc_index > 0 || c == '\033') {
        if (c == '\033') {
            esc_index = 0;
        }
        esc_buffer[esc_index++] = c;
        if (esc_index == 3) {
            if (esc_buffer[0] == '\033' && esc_buffer[1] == '[') {
                if (esc_buffer[2] == 'D') {
                    // Left arrow
                    if (cursor_pos > 0) {
                        cursor_pos--;
                        uart_send_string(UART0, "\033[D");
                    }
                } else if (esc_buffer[2] == 'C') {
                    // Right arrow
                    if (cursor_pos < shell_index) {
                        cursor_pos++;
                        uart_send_string(UART0, "\033[C");
                    }
                } else if (esc_buffer[2] == 'A') {
                    // Up arrow
                    if (history_count > 0) {
                        if (history_index < history_count - 1) {
                            history_index++;
                        }
                        strcpy(shell_buffer, command_history[history_count - 1 - history_index]);
                        shell_index = strlen(shell_buffer);
                        cursor_pos = shell_index;
                        redraw_line();
                    }
                } else if (esc_buffer[2] == 'B') {
                    // Down arrow
                    if (history_index >= 0) {
                        history_index--;
                        if (history_index < 0) {
                            shell_buffer[0] = '\0';
                            shell_index = 0;
                            cursor_pos = 0;
                        } else {
                            strcpy(shell_buffer, command_history[history_count - 1 - history_index]);
                            shell_index = strlen(shell_buffer);
                            cursor_pos = shell_index;
                        }
                        redraw_line();
                    }
                }
            }
            esc_index = 0;
        }
        return;
    }

    // --- Handle backspace ---
    if (c == 0x7F || c == '\b') {
        if (cursor_pos > 0) {
            for (i = cursor_pos - 1; i < shell_index - 1; i++) {
                shell_buffer[i] = shell_buffer[i + 1];
            }
            shell_index--;
            cursor_pos--;
            shell_buffer[shell_index] = '\0';
            redraw_line();
        }
    }
    // --- Handle Enter key ---
    else if (c == '\r' || c == '\n') {
        shell_buffer[shell_index] = '\0';
        uart_send_string(UART0, "\r\n");
        shell_process_command(shell_buffer);
        shell_index = 0;
        cursor_pos = 0;
        shell_buffer[0] = '\0';
    }
    // --- Handle printable characters ---
    else if (c >= 0x20 && c <= 0x7E) {
        if (shell_index < SHELL_BUFFER_SIZE - 1) {
            if (cursor_pos == shell_index) {
                shell_buffer[shell_index++] = c;
                cursor_pos++;
                shell_buffer[shell_index] = '\0';
                uart_send(UART0, c);
            } else {
                for (i = shell_index; i > cursor_pos; i--) {
                    shell_buffer[i] = shell_buffer[i - 1];
                }
                shell_buffer[cursor_pos] = c;
                shell_index++;
                cursor_pos++;
                shell_buffer[shell_index] = '\0';
                redraw_line();
            }
        }
    }
}

void shell_init() {
    exit_shell = 0;
    shell_index = 0;
    cursor_pos = 0;
    shell_buffer[0] = '\0';
    history_count = 0;
    history_index = -1;
    uart_send_string(UART0, "> ");
}

void shell_read_listener(void) {
    while (active_process && !ring_empty(&active_process->context.ring)) {
        const char c = ring_get(&active_process->context.ring);
        shell_process_char(c);
    }
}

void shell_start(void* arg) {
    shell_init();
    while (1) {
        if (exit_shell) break;
        core_halt();
    }
}
