#include "error_handler.h"
#include <isr.h>
#include "uart.h"
#include "string.h"

void panic() {
    core_disable_irqs();
    for (;;);
}

void sys_exit(const int code_no, const char *msg) {
  if (code_no == 0) {
    uart_send_string(UART0, "\r\nSystem exit: ");
    uart_send_string(UART0, msg);
    uart_send_string(UART0, "\r\n");
  } else {
    char code_str[INT_TO_STRING_MAX];
    int_to_string(code_no, code_str);
    uart_send_string(UART0, "\r\nSystem error (");
    uart_send_string(UART0, code_str);
    uart_send_string(UART0, "): ");
    uart_send_string(UART0, msg);
    uart_send_string(UART0, "\r\n");
  }

  panic();
}

