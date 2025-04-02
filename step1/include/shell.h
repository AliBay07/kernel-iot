#ifndef SHELL_H
#define SHELL_H

/*
 * Processes a complete command line that has been entered at the shell.
 */
void shell_process_command(const char *cmd);

/*
 * Processes a single character input from the terminal.
 * This function accumulates input into an internal buffer.
 * When the Enter key is pressed, it calls shell_process_command().
 */
void shell_process_char(char c);

/*
 * Initialize the shell.
 * This function is called once at startup.
 */
void shell_init();

/*
 * Processes the shell input.
 */
void shell_process();

#endif //SHELL_H
