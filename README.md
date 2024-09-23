# Minishell Project README

---

## 1. Introduction

Minishell is a 42 school project that involves building a simple Unix shell from scratch. The goal is to replicate core functionalities of bash or sh, providing users with an interactive prompt where they can execute commands, handle redirection, and manage pipes, signals, and environment variables. Additionally, we extend the project with unit tests and a strong development methodology based on extreme programming principles.

This README will serve as a comprehensive guide to mastering the Minishell project, explaining our approach, environment, project structure, and development strategy.

---

## 2. Requirements

### Mandatory Features

The project requires implementing the following features:

- **Prompt display**: Show a prompt to receive user input.
- **Command history**: Maintain a history of commands that the user can navigate.
- **Command execution**: Run executables based on the `PATH` variable or specified absolute/relative paths.
- **Redirections**: Handle:
  - `>`: Output redirection.
  - `<`: Input redirection.
  - `>>`: Append output redirection.
  - `<<`: Here-document redirection.
- **Pipes**: Connect commands using `|` to pipe outputs between commands.
- **Built-in commands**:
  - `echo` with `-n`.
  - `cd` to change directories.
  - `pwd` to print the working directory.
  - `export` to set environment variables.
  - `unset` to unset environment variables.
  - `env` to list environment variables.
  - `exit` to close the shell.
- **Signal handling**:
  - `Ctrl-C`: Interrupt and show a new prompt.
  - `Ctrl-D`: Exit the shell.
  - `Ctrl-\`: Ignore.

### Bonus Features
- **Logical Operators**: Support `&&` and `||` for conditional execution of commands.
- **Wildcards**: Handle `*` to match files in the current directory.

---

## 3. Development Environment

### IDE Configuration

Our development environment revolves around Vim 9.1, a highly customizable text editor, which we have tailored with various plugins to support C development, testing, and debugging. This configuration enables fast, efficient, and flexible development with real-time feedback and streamlined Git integration.

Here’s the list of the tools and plugins we use:

1. **Vim 9.1 with server-client, clipboard, and Python3 support**: This version supports multi-file editing, server-client setups for debugging, and clipboard integration for copying and pasting text across applications.

2. **vim-rooter**: Automatically sets the working directory to the root of the project, ensuring that file paths remain consistent across different sessions.

3. **Nerd Tree**: A file explorer for navigating through project directories. It provides an overview of the project’s structure.

4. **Fzf Vim**: A fuzzy finder for quickly locating files, lines, or text within files. It's an essential tool for large codebases.

5. **vim tagbar**: A sidebar displaying tags (functions, variables, etc.) within the current file, improving navigation and understanding of large source files.

6. **Airline status bar**: Provides a sleek and informative status bar, showing file type, git branch, position in file, and other useful information.

7. **vim-fugitive**: Git wrapper for Vim. It allows seamless Git integration within Vim, making it easy to run Git commands directly from the editor.

8. **vim GV**: A Git log visualizer. It presents the commit history in a user-friendly way, allowing us to review commits and understand changes over time.

9. **vim-flog**: A visual interface for Git trees, helping us to understand branch structures and changes across multiple branches.

10. **vim ALE**: A linter that provides syntax checking and quickfix options for various languages, including C, which helps ensure code correctness.

11. **vim-buffet**: Enhanced buffer line management for efficiently handling multiple open files within the editor.

12. **YouCompleteMe (YCM)**: A code completion plugin that uses clangd as the compiler backend for C projects, providing real-time code suggestions and error checking.

13. **Vimspector**: A debugging plugin that supports stepping through code, inspecting variables, and managing breakpoints, with support for std lib debugging tools.

14. **Norminette plugin**: A plugin that checks 42 school’s coding norms directly inside Vim, ensuring compliance with school standards.

15. **Test Visualizer Plugin**: A custom plugin that integrates with Check tests compiled with CMake and CTest. It displays visual indicators (green/red) to show whether tests passed or failed, and provides a popup window with detailed test logs.

16. **Makefile Interface**: Prompts makefile commands, allowing management of the entire project within Vim. It also offers help sections and extra features for efficiency.

### IDE Benefits

- **Speed**: The plugins enhance our ability to navigate, edit, and compile code efficiently.
- **Portability**: We can use this setup on different machines and environments, ensuring a consistent experience.
- **Real-time Feedback**: With ALE and the test visualizer plugin, we receive immediate feedback on code correctness and test results.
- **Seamless Git Integration**: Plugins like vim-fugitive, vim-flog, and vim GV enable smooth version control directly within the editor, improving team collaboration.

---

## 4. Project Structure

We follow a standardized project layout to ensure consistency and collaboration efficiency. The structure is designed to facilitate navigation, encourage modularity, and simplify testing.

### Directory Layout:

```
└── Project Root
    ├── include/
    │   ├── project.h               # Main project header linking all sub-headers
    │   ├── defines.h               # Optional: Define macros, constants
    ├── lib/
    │   ├── libfoo.a                # Example static library
    │   └── libbar.so               # Example shared library
    ├── assets/
    │   ├── logo.png                # Project logo or other images
    │   └── config.json             # Configuration files or templates
    ├── src/
    │   ├── sub_folder_name1/
    │   │   ├── function_name1.c    # Source file for function
    │   │   ├── module1.h           # Sub-folder header
    │   └── function_name2.c        # Another function
    ├── test_src/
    │   ├── CMakeLists.txt          # Managed by VisuTest for test integration
    │   ├── sub_folder_name1/
    │   │   └── test_function_name1.c  # Test file matching source
    │   ├── test_function_name2.c   # Another test file
    ├── ycm_extra_conf.py           # Configuration for YouCompleteMe plugin
    ├── .vimspector.json            # Configuration file for Vimspector plugin
    ├── Makefile                    # Build file
    └── .gitignore                  # List of files/folders to ignore in Git
```

### Key Rules:
- **Single Function per File**: Each `.c` file contains only one public function, with possible helper functions inside. This improves clarity, debugging, and modularity.
- **Multiple Header Files**: We allow multiple `.h` files across subfolders, but they must all be linked in the main `project.h` for simplicity and clarity in compilation.
- **Test Files**: For every `.c` file in `src/`, a corresponding test file is created in `test_src/`, prefixed with `test_`.

### Example: Directory Structure for Command Implementations
Let’s say you are implementing built-in commands such as `cd` and `echo`:

```
└── Project Root
    ├── src/
    │   ├── builtins/
    │   │   ├── cd.c                # cd command implementation
    │   │   ├── echo.c              # echo command implementation
    ├── test_src/
    │   ├── builtins/
    │   │   ├── test_cd.c           # Test for cd command
    │   │   ├── test_echo.c         # Test for echo command
```

Each function gets its own `.c` file and test, allowing clear and organized management of source code and tests.

---

## 5. Tools and Frameworks

### Check + CTest
- **Check Framework**: Used for writing unit tests to verify individual functions.
- **CTest**: Runs the tests in an organized manner and integrates with our Makefile for continuous testing.

**Example Test**:
To test the `cd` command implementation:

```c
void test_cd() {
    // Prepare arguments
    char *args[] = {"cd", "/home/user", NULL};
    // Execute the command
    int result = shell_execute(args);
    // Verify the result
    cr_assert_eq(result, 0); // Assuming success returns 0
    cr_assert_str_eq(getcwd(NULL, 0), "/home/user"); // Verify directory changed
}
```

### readline
- **readline Library**: Handles reading user input, allowing command line history and editing capabilities.
  
**Example**:
We use `add_history()` to store each command entered by the user:

```c
char *input = readline("minishell> ");
add_history(input);
```

### Personal Libraries

#### Libft
Our own utility library, `libft`, contains essential functions for memory management, string manipulation, and data structures. Here’s the header file:

```c
#ifndef LIBFT_H
# define LIBFT_H

// Standard includes
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

// Function prototypes
int ft_atoi(const char *nptr);
char *ft_strdup(const char *s);
void ft_putchar_fd(char c, int fd);
void *ft_calloc(size_t nmemb, size_t size);
char **ft_split(char const *s, char c);
#endif /*LIBFT_H*/
```

#### libgc
The garbage collector we created to manage memory automatically. This library includes functions to allocate memory and clean it up at the end of the program.

**Key Functions**:
- `gc_malloc(size_t size)`: Allocates memory and registers it.
- `gc_collect(void)`: Cleans up all unlocked pointers to prevent memory leaks.

Here’s the header file for `libgc`:

```c
#ifndef LIBGC_H
# define LIBGC_H

typedef struct s_gc_node {
    void *ptr;
    struct s_gc_node *next;
} t_gc_node;

void *gc_malloc(size_t size);
void gc_collect(void);

#endif /*LIBGC_H*/
```

### Example Usage of libgc:
To allocate memory for a string safely:

```c
char *str = gc_malloc(50 * sizeof(char)); // Allocate 50 bytes
strcpy(str, "Hello, World!"); // Use allocated memory
```

---

## 6. Development Strategy

### Pair Programming Approach
We use extreme programming techniques to maximize collaboration and code quality:

- **Driver and Navigator Roles**: One person writes code (Driver) while the other reviews and suggests improvements (Navigator). After a set period, roles switch.
- **Continuous Feedback**: Regular check-ins and discussions about the code being written help ensure alignment and understanding.

### Git Workflow
- **Branching Strategy**:
  - Feature branches: Each new feature gets a dedicated branch (e.g., `feature/cd-command`).
  - Bug fix branches: Issues are addressed on separate branches (e.g., `bugfix/memory-leak`).

- **Pull Request Reviews**: Submit pull requests for code review. Include descriptive comments, addressing what was changed and why.

### Testing-Driven Development (TDD)
1. **Write the Test First**: Begin by writing a unit test for the desired functionality.
2. **Implement the Code**: Write the code to make the test pass.
3. **Refactor**: Clean up the code while ensuring all tests still pass.

### Example TDD Cycle
1. **Write a Test for echo**:
```c
void test_echo() {
    char *args[] = {"echo", "Hello World", NULL};
    int result = shell_execute(args);
    cr_assert_eq(result, 0); // Test should pass if the command executes successfully
}
```

2. **Implement the echo Command**:
```c
int shell_execute(char **args) {
    if (strcmp(args[0], "echo") == 0) {
        printf("%s\n", args[1]); // Print the argument
        return 0; // Success
    }
    return -1; // Unknown command
}
```

3. **Refactor and Run Tests**: Ensure all tests pass, then clean up the code if necessary.

---
