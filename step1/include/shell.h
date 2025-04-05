#ifndef SHELL_H
#define SHELL_H

/*
 * Starts the shell.
 * This function is called once at startup.
 */
void shell_start(void* arg);

/*
 * Read listener for the shell program.
 */
void shell_read_listener(void);

#endif //SHELL_H
