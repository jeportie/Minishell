# Garbage Collection Library

## Overview

This library provides an efficient garbage collection mechanism for managing memory allocation and resource cleanup in C programs. The system offers automatic memory management and file descriptor registration, making dynamic memory handling simpler and safer. It includes a locking mechanism for memory cleanup that allows for selective memory retention during the program's execution. The library is designed to allow memory to be freed during the program runtime to avoid allocation overflow.

## Features

- **Automatic Memory Management**: Handles memory allocation and deallocation using a custom garbage collection strategy.
- **File Descriptor Tracking**: Manages and cleans up file descriptors as part of the garbage collection process.
- **Memory Locking for Cleanup**: The locking mechanism allows data to be selectively protected from cleanup during the program's execution. When `gc_collect` is called, it will free and destroy all data that is not locked. This feature is useful for temporary allocations that do not need to persist throughout the entire program.
- **Garbage Cleanup**: The `gc_cleanup` function will erase all remaining locked data after the program finishes or when it is safe to do so, allowing for proper management and freeing of resources during the program.

## File Descriptions

- **gc_cleanup.c**: Provides the functionality to clean up and free all resources that were locked during the program's execution.
- **gc_collect.c**: Implements the garbage collection routine that collects and frees unused memory, freeing only data that is not locked.
- **gc_fd_register.c**: Handles the registration of file descriptors and ensures they are closed and cleaned up during garbage collection.
- **gc_globals.c**: Defines and manages the global state required for tracking allocated memory and file descriptors.
- **gc_malloc.c**: A custom memory allocation function that works in tandem with the garbage collection system to allocate memory.
- **gc_mark_lock.c**: Implements the memory locking mechanism, allowing certain data to be retained during `gc_collect` until it is safe to free.
- **gc_nest_register.c**: Manages the nested registration of resources, allowing complex hierarchies of resource ownership.
- **gc_register.c**: Provides the main interface for registering memory and resources with the garbage collector.
- **get_next_line.c**: A utility function that reads a line from a file descriptor and registers the allocated buffer for garbage collection.

## Usage

To use this library, include the relevant header files and link the source files in your project. You can use the `gc_malloc` and `gc_register` functions to manage memory allocation. Call `gc_collect` to free all non-locked memory during the program execution, and `gc_cleanup` at the end to clean up locked resources.

## Installation

1. Clone the repository containing this library:
   ```bash
   git clone https://github.com/your-repo/garbage-collection-lib.git
   cd garbage-collection-lib
   make
   ```

2. In your project, include the library headers and link the compiled library:
   - **Include** the necessary headers in your source code:
     ```c
     #include "gc_lib.h"
     #include "gc_register.h"
     ```
   - **Link the library** during compilation. Add the include and library flags:
     ```bash
     gcc -o my_program main.c -L/path/to/lib -lgc
     ```

   Replace `/path/to/lib` with the path to where the compiled library resides.

3. Compile and run your project as usual, ensuring you link the garbage collection library as shown.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
