# Minimal Embedded Kernel in C

This project is a simple embedded kernel written in C. It includes basic memory management, process creation, interrupt handling, and a shell interface that runs through UART.

The kernel supports:

- A custom memory allocator
- Process creation and execution
- Ring buffer-based I/O for each process
- UART interrupts
- A basic shell as a user process
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
