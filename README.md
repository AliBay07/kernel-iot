# Minimal Embedded Kernel in C

This project is a simple embedded kernel written in C. It includes basic memory management, task creation, interrupt and event handling, and a shell interface that runs through UART.

The kernel supports:

- A custom memory allocator
- Task creation and activation
- Ring buffer-based I/O for each process
- UART interrupts
- Event handling
- A basic shell as a user task
- Listener functions for reading from input buffers

## Running the project

To build the project, use the provided Makefile:

```bash
make
```

You can run the kernel in an emulator using:

```bash
make run
```

### Requirements

To run the project in an emulator, make sure you have the following installed:

- qemu-system-arm (for emulating the target hardware)
- arm-none-eabi-gcc and related tools (for cross-compilation)
- make and gdb (for building and debugging)
