#ifndef SHELL_H
#define SHELL_H
#include <task.h>


/*
 * Read listener for the shell program.
 */
void shell_read_listener(void* arg);

/*
 * Write listener for the shell program.
 */
void shell_write_listener(void* arg);

/*
 * This function is called when the shell program
 * is initialized.
 */
void shell_init(t_context_t* context, void* arg);

#endif //SHELL_H
