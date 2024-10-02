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
### Allowed External Functions

In the Minishell project, we are allowed to use a wide range of external functions, categorized into specific groups for ease of reference:

1. **Readline Library Functions**:
   - `readline`: Reads a line of input with line editing capabilities.
   - `rl_clear_history`: Clears the command history.
   - `rl_on_new_line`: Updates the state of the input line after editing.
   - `rl_replace_line`: Replaces the current input line with a new string.
   - `rl_redisplay`: Refreshes the input line display after changes.
   - `add_history`: Adds the most recent command to the command history.

2. **Input/Output and Memory Functions**:
   - `printf`: Prints formatted output to the console.
   - `malloc`: Allocates memory dynamically.
   - `free`: Frees dynamically allocated memory.
   - `write`: Writes data to a file descriptor.
   - `read`: Reads data from a file descriptor.
   - `open`: Opens a file.
   - `close`: Closes a file descriptor.
   - `access`: Checks accessibility of a file.

3. **Process Management**:
   - `fork`: Creates a new process.
   - `wait`: Waits for a child process to change state.
   - `waitpid`, `wait3`, `wait4`: Waits for a process with additional options.
   - `execve`: Executes a program, replacing the current process image.
   - `kill`: Sends a signal to a process.
   - `exit`: Exits the current process with a status.

4. **Signal Handling**:
   - `signal`: Sets a signal handler for a given signal.
   - `sigaction`: Examines and changes the action taken by a signal.
   - `sigemptyset`, `sigaddset`: Initializes and manipulates signal sets.

5. **File Management and Status**:
   - `getcwd`: Gets the current working directory.
   - `chdir`: Changes the current working directory.
   - `stat`, `lstat`, `fstat`: Retrieves information about files.
   - `unlink`: Removes a file.
   - `opendir`, `readdir`, `closedir`: Handles directory reading.

6. **File Descriptors and Duplication**:
   - `dup`, `dup2`: Duplicates file descriptors.
   - `pipe`: Creates a pipe for inter-process communication.

7. **Error Handling**:
   - `strerror`: Returns a string describing the error number.
   - `perror`: Prints a description of the last error.

8. **Terminal and Device Control**:
   - `isatty`: Checks if a file descriptor refers to a terminal.
   - `ttyname`: Returns the name of the terminal associated with a file descriptor.
   - `ttyslot`: Returns the index of the current terminal.
   - `ioctl`: Controls the I/O settings of file descriptors.

9. **Environment Variables**:
   - `getenv`: Retrieves the value of an environment variable.

10. **Terminal Control Functions**:
    - `tcsetattr`, `tcgetattr`: Sets and gets terminal attributes.
    - `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`: Handles terminal capability information.
    - `tgoto`: Moves the cursor to a specified position.
    - `tputs`: Sends control strings to the terminal.

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


### 1. **What is an AST?**

An **Abstract Syntax Tree (AST)** is a tree representation of the structure of a program's source code. Each node of the tree corresponds to a construct occurring in the source code. In the context of a shell, the AST is used to represent user commands, operators (pipes, redirections), and control structures (like logical operators `&&`, `||`).

**Why develop an AST?**
- **Efficiency**: The AST simplifies execution by transforming input into a hierarchical structure that can be traversed for command execution.
- **Modularity**: Each node can represent a different type of operation, allowing clean separation of logic.
- **Extendability**: An AST is flexible, making it easier to add new features like logical operators or custom commands in the future.
- **Error Handling**: Parsing errors can be identified early in the process.

---

### 2. **AST Structure in Minishell**

The AST for your Minishell needs to represent user commands and their relations (pipes, logical operators, redirections). Here’s how we’ll structure it:

- **Command Node**: Represents a command like `ls`, `echo`, etc.
- **Pipe Node**: Represents a pipe (`|`), connecting two commands.
- **Redirection Node**: Represents input/output redirections (`>`, `<`, `>>`, `<<`).
- **Logical Node**: Represents logical operators (`&&`, `||`), used for conditional command execution.
  
### Example:

Let’s take an example command:

```bash
cat file.txt | grep "pattern" && echo "Found"
```

The corresponding AST would look like this:

```
        &&
       /  \
      |    echo
     / \
  cat  grep
```

- **`cat file.txt`** and **`grep "pattern"`** are connected by a **Pipe Node**.
- The output of the **Pipe Node** is connected to the logical **&&** operator.
- The **echo** command is executed only if the pipeline on the left succeeds.

---

### 3. **Subshells vs Classic AST**

#### **Classic AST**:
- **AST** is a straightforward hierarchical structure of commands and operators.
- It allows structured, sequential execution of commands.
- Easier to extend and manage as all nodes (commands, operators) are clearly defined and interconnected.

#### **Subshells**:
- A subshell is a child process that runs its own shell, allowing isolated command execution. Subshells are commonly used for grouping commands (`(cmd1; cmd2)`), backgrounding processes (`cmd &`), or handling redirections.
- **Subshells** can introduce complications, especially for variable handling, as variables set in a subshell don’t affect the parent shell.
- Using subshells can simplify some parsing cases, such as when grouping commands with parentheses, but it adds overhead and complexity to the AST and execution model.

#### **Parsing Strategy Decision**:
- **Classic AST** will be our primary approach. It’s efficient and will handle most of the shell’s requirements.
- **Subshells** will be implemented only when necessary, such as when handling command grouping (e.g., `(cmd1; cmd2)`).
- This keeps the codebase cleaner, prevents unnecessary process creation, and simplifies variable management.

---

### 4. **Common Bugs, Exceptions, and Corner Cases**

Several well-known issues arise when implementing a shell with an AST:

1. **Command Grouping**:
   - Command sequences grouped with parentheses (e.g., `(cmd1; cmd2)`) are executed in a subshell. We need to account for grouping in the AST.

   **Example**:
   ```bash
   (echo "Hello"; echo "World")
   ```
   Here, both commands are executed in a subshell, meaning any environment changes don’t affect the parent shell.

2. **Error Handling with Redirections**:
   - Redirections often produce tricky bugs when improperly handled.
   - Example: `echo hello > file > anotherfile`. The shell should raise an error as multiple output redirections are invalid.

3. **Multiple Logical Operators**:
   - Handling mixed logical operators (`&&` and `||`) in one command needs careful precedence rules.
   
   **Example**:
   ```bash
   cmd1 && cmd2 || cmd3
   ```
   The AST should ensure `cmd2` is only executed if `cmd1` succeeds, but `cmd3` is executed if `cmd2` fails.

4. **Here-documents (`<<`)**:
   - `<<` requires special handling to capture multiline input until a delimiter is reached. This is an edge case that can introduce bugs if not parsed correctly.

5. **Wildcards (`*`)**:
   - Implementing wildcard expansion involves additional processing before the AST is built, requiring integration with directory reading functions (`opendir`, `readdir`).

6. **Background Processes**:
   - Commands executed with `&` run in the background. We’ll need to handle this by forking properly and not waiting for the background process to complete.

---

### 5. **Real-World Use of ASTs**

ASTs are used extensively in programming languages, compilers, and shells for their flexibility and structured representation of complex syntaxes.

- **Compilers**: Languages like C, Python, and Java use ASTs to represent source code. After parsing the code into an AST, the compiler can analyze and generate machine code.
- **Bash and Zsh**: Both use ASTs internally to manage command execution, redirections, and pipes efficiently.
- **Programming Editors**: ASTs are used in editors for syntax highlighting, autocompletion, and error detection.

---

### 6. **Why Develop an AST for a Shell?**

Developing an AST is crucial for:

- **Managing Complexity**: Shell commands often have complex syntax, including nested operations, pipes, and logical operators. The AST simplifies managing this complexity.
- **Execution Optimization**: ASTs allow for optimization during execution. For example, short-circuit evaluation for logical operators (`&&`, `||`) can be easily implemented with an AST.
- **Extendability**: An AST can be extended to support new shell features, like additional logical operators or command grouping, without significant code restructuring.

---

### Schematics and Diagrams

Here’s a visual representation of a few example ASTs:

Certainly! I'll keep the exact use cases and diagrams, and for each one, I'll add a descriptive text explaining the strategy to employ with the AST for that case.

---

### 1. **Simple Command**

**Command Line:**

```bash
ls
```

**AST Diagram:**

```
[COMMAND]
   |
 "ls"
```

**AST Strategy:**

- **Node Type**: `COMMAND` node.
- **Structure**: Since there are no arguments or operators, the AST consists of a single node representing the `ls` command.
- **Parsing Strategy**: Create a `COMMAND` node with the command name `ls`.
- **Execution Strategy**: Execute the `ls` command directly.

---

### 2. **Command with Arguments**

**Command Line:**

```bash
echo "Hello, World!"
```

**AST Diagram:**

```
[COMMAND]
   |
 "echo"
   |
"Hello, World!"
```

**AST Strategy:**

- **Node Type**: `COMMAND` node with arguments.
- **Structure**: The `COMMAND` node contains the command `echo` and its argument `"Hello, World!"`.
- **Parsing Strategy**: Tokenize the input to identify the command and its arguments, then create a `COMMAND` node with this information.
- **Execution Strategy**: Pass the arguments to the `echo` command during execution.

---

### 3. **Command with Input Redirection**

**Command Line:**

```bash
sort < unsorted.txt
```

**AST Diagram:**

```
 [REDIRECT_IN]
      /      \
[COMMAND]   "unsorted.txt"
    |
  "sort"
```

**AST Strategy:**

- **Node Types**: `REDIRECT_IN` node with a `COMMAND` child and a filename.
- **Structure**: The `REDIRECT_IN` node indicates input redirection, where `sort` reads from `unsorted.txt`.
- **Parsing Strategy**: Identify the `<` operator, create a `REDIRECT_IN` node, and link the `COMMAND` node and the filename.
- **Execution Strategy**: Before executing `sort`, redirect its standard input to read from `unsorted.txt`.

---

### 4. **Command with Output Redirection**

**Command Line:**

```bash
ls > files.txt
```

**AST Diagram:**

```
 [REDIRECT_OUT]
      /      \
[COMMAND]  "files.txt"
    |
  "ls"
```

**AST Strategy:**

- **Node Types**: `REDIRECT_OUT` node with a `COMMAND` child and a filename.
- **Structure**: The `REDIRECT_OUT` node represents output redirection to `files.txt`.
- **Parsing Strategy**: Detect the `>` operator, create a `REDIRECT_OUT` node, and associate it with the `COMMAND` node and the target file.
- **Execution Strategy**: Redirect the standard output of `ls` to write to `files.txt`.

---

### 5. **Command with Append Output Redirection**

**Command Line:**

```bash
echo "New Entry" >> log.txt
```

**AST Diagram:**

```
 [APPEND_OUT]
      /      \
[COMMAND]  "log.txt"
    |
 "echo"
    |
"New Entry"
```

**AST Strategy:**

- **Node Types**: `APPEND_OUT` node with a `COMMAND` child and a filename.
- **Structure**: The `APPEND_OUT` node indicates that the output should be appended to `log.txt`.
- **Parsing Strategy**: Recognize the `>>` operator, build an `APPEND_OUT` node, and link it with the command and the file.
- **Execution Strategy**: Execute `echo "New Entry"` and append the output to `log.txt`.

---

### 6. **Command with Pipe**

**Command Line:**

```bash
ls | grep "test"
```

**AST Diagram:**

```
     [PIPE]
     /    \
[COMMAND][COMMAND]
   |        |
 "ls"    "grep"
           |
        "test"
```

**AST Strategy:**

- **Node Type**: `PIPE` node connecting two `COMMAND` nodes.
- **Structure**: Left child is the `ls` command; right child is `grep "test"`.
- **Parsing Strategy**: Identify the `|` operator, create a `PIPE` node, and attach the two commands as its children.
- **Execution Strategy**: Execute `ls`, pipe its output to `grep "test"`.

---

### 7. **Command with Multiple Pipes**

**Command Line:**

```bash
cat file.txt | grep "error" | sort
```

**AST Diagram:**

```
         [PIPE]
        /     \
   [PIPE]    [COMMAND]
   /   \          |
[COMMAND][COMMAND]"sort"
   |       |
 "cat"   "grep"
   |       |
"file.txt""error"
```

**AST Strategy:**

- **Node Types**: Nested `PIPE` nodes.
- **Structure**: The first `PIPE` node connects `cat file.txt` and `grep "error"`; the second `PIPE` node connects this result to `sort`.
- **Parsing Strategy**: Parse from left to right, building `PIPE` nodes as you encounter `|`, nesting them appropriately.
- **Execution Strategy**: Set up a pipeline where the output flows from `cat` to `grep`, then to `sort`.

---

### 8. **Command with Input and Output Redirection**

**Command Line:**

```bash
grep "search" < input.txt > output.txt
```

**AST Diagram:**

```
     [REDIRECT_OUT]
        /        \
 [REDIRECT_IN] "output.txt"
    /      \
[COMMAND] "input.txt"
    |
  "grep"
    |
 "search"
```

**AST Strategy:**

- **Node Types**: `REDIRECT_OUT` node with a `REDIRECT_IN` child.
- **Structure**: Input redirection is applied first, then output redirection.
- **Parsing Strategy**: Build the `REDIRECT_IN` node when `<` is found, then the `REDIRECT_OUT` node upon encountering `>`, nesting them accordingly.
- **Execution Strategy**: Redirect standard input from `input.txt`, then standard output to `output.txt`, before executing `grep "search"`.

---

### 9. **Command with Here-Document**

**Command Line:**

```bash
cat << EOF
Hello World
EOF
```

**AST Diagram:**

```
   [HEREDOC]
    /     \
[COMMAND] "EOF"
    |
  "cat"
```

**AST Strategy:**

- **Node Type**: `HEREDOC` node with a `COMMAND` child and a delimiter.
- **Structure**: The `HEREDOC` node holds the content provided until the `EOF` delimiter.
- **Parsing Strategy**: Recognize `<<`, create a `HEREDOC` node, read input until `EOF`, and associate it with the command.
- **Execution Strategy**: Provide the here-document content as standard input to `cat`.

---

### 10. **Command with Logical AND**

**Command Line:**

```bash
make && make clean
```

**AST Diagram:**

```
      [AND]
     /     \
[COMMAND][COMMAND]
   |         |
 "make"   "make"
            |
          "clean"
```

**AST Strategy:**

- **Node Type**: `AND` node connecting two `COMMAND` nodes.
- **Structure**: The left child is `make`, the right child is `make clean`.
- **Parsing Strategy**: Upon encountering `&&`, create an `AND` node and link the preceding and following commands.
- **Execution Strategy**: Execute `make`; if it succeeds, execute `make clean`.

---

### 11. **Command with Logical OR**

**Command Line:**

```bash
git commit || echo "Commit failed"
```

**AST Diagram:**

```
      [OR]
     /    \
[COMMAND][COMMAND]
   |        |
 "git"   "echo"
   |        |
"commit""Commit failed"
```

**AST Strategy:**

- **Node Type**: `OR` node connecting two `COMMAND` nodes.
- **Structure**: Left child is `git commit`, right child is `echo "Commit failed"`.
- **Parsing Strategy**: Identify `||`, create an `OR` node, and attach the commands.
- **Execution Strategy**: Execute `git commit`; if it fails, execute `echo "Commit failed"`.

---

### 12. **Command with Combined Logical Operators**

**Command Line:**

```bash
make && make test || echo "Tests failed"
```

**AST Diagram:**

```
          [OR]
         /    \
      [AND] [COMMAND]
     /     \     |
[COMMAND][COMMAND]"echo"
   |         |    |
"make"     "make" "Tests failed"
             |
           "test"
```

**AST Strategy:**

- **Node Types**: `AND` node connected to an `OR` node.
- **Structure**: `make && make test` forms the `AND` node; the result connects to `echo` via the `OR` node.
- **Parsing Strategy**: Parse `&&` and `||`, respecting operator precedence (usually left to right for same precedence).
- **Execution Strategy**: Execute `make`; if it succeeds, execute `make test`; if either fails, execute `echo "Tests failed"`.

---

### 13. **Command with Background Execution** *(Note: Bonus Feature)*

**Command Line:**

```bash
sleep 60 &
```

**AST Diagram:**

```
[BACKGROUND]
     |
 [COMMAND]
     |
  "sleep"
     |
   "60"
```

**AST Strategy:**

- **Node Type**: `BACKGROUND` node with a `COMMAND` child.
- **Structure**: The `BACKGROUND` node indicates the command should run in the background.
- **Parsing Strategy**: Recognize `&` at the end, create a `BACKGROUND` node, and attach the command.
- **Execution Strategy**: Execute `sleep 60` in a child process without waiting for it to complete.

---

### 14. **Command with Multiple Background Commands**

**Command Line:**

```bash
cmd1 & cmd2 &
```

**AST Diagram:**

```
[BACKGROUND]
     |
 [COMMAND]
     |
  "cmd1"

[BACKGROUND]
     |
 [COMMAND]
     |
  "cmd2"
```

**AST Strategy:**

- **Node Types**: Separate `BACKGROUND` nodes for each command.
- **Structure**: Each command followed by `&` creates its own `BACKGROUND` node.
- **Parsing Strategy**: Parse each command individually, creating a `BACKGROUND` node when `&` is encountered.
- **Execution Strategy**: Execute `cmd1` and `cmd2` concurrently in the background.

---

### 15. **Command with Subshell Execution**

**Command Line:**

```bash
(echo "Start"; ls)
```

**AST Diagram:**

```
 [SUBSHELL]
     |
 [SEQUENCE]
    /     \
[COMMAND][COMMAND]
   |         |
 "echo"     "ls"
    |
 "Start"
```

**AST Strategy:**

- **Node Type**: `SUBSHELL` node containing a `SEQUENCE` of commands.
- **Structure**: Commands inside parentheses are grouped under a `SUBSHELL` node.
- **Parsing Strategy**: Upon encountering `(`, create a `SUBSHELL` node and parse enclosed commands as a sequence.
- **Execution Strategy**: Execute the commands in a new subshell, so any environment changes do not affect the parent shell.

---

### 16. **Command with Pipe and Redirection**

**Command Line:**

```bash
ps aux | grep "python" > processes.txt
```

**AST Diagram:**

```
      [REDIRECT_OUT]
         /        \
      [PIPE]   "processes.txt"
      /    \
[COMMAND][COMMAND]
   |        |
 "ps"    "grep"
   |        |
 "aux"   "python"
```

**AST Strategy:**

- **Node Types**: `PIPE` node connected to a `REDIRECT_OUT` node.
- **Structure**: The pipeline output is redirected to `processes.txt`.
- **Parsing Strategy**: Build the `PIPE` node first, then upon encountering `>`, create a `REDIRECT_OUT` node.
- **Execution Strategy**: Execute `ps aux`, pipe to `grep "python"`, and redirect the final output to `processes.txt`.

---

### 17. **Command with Wildcard Expansion**

**Command Line:**

```bash
ls *.c
```

**AST Diagram:**

```
[COMMAND]
   |
 "ls"
   |
 "*.c"
```

**AST Strategy:**

- **Node Type**: `COMMAND` node with wildcard arguments.
- **Structure**: The argument `*.c` is subject to wildcard expansion.
- **Parsing Strategy**: Before building the AST, perform wildcard expansion to replace `*.c` with matching filenames.
- **Execution Strategy**: Execute `ls` with the list of `.c` files obtained from the expansion.

---

### 18. **Command with Multiple Redirections**

**Command Line:**

```bash
grep "error" < input.txt >> output.log
```

**AST Diagram:**

```
      [APPEND_OUT]
         /      \
  [REDIRECT_IN]"output.log"
     /      \
[COMMAND] "input.txt"
    |
  "grep"
    |
 "error"
```

**AST Strategy:**

- **Node Types**: `REDIRECT_IN` node inside an `APPEND_OUT` node.
- **Structure**: Input is redirected from `input.txt`, and output is appended to `output.log`.
- **Parsing Strategy**: Parse `<` and build `REDIRECT_IN`, then parse `>>` and wrap the existing node with `APPEND_OUT`.
- **Execution Strategy**: Set up both input and output redirections before executing `grep "error"`.

---

### 19. **Command with Logical AND and Pipes**

**Command Line:**

```bash
cmd1 | cmd2 && cmd3
```

**AST Diagram:**

```
      [AND]
     /     \
  [PIPE] [COMMAND]
   /   \      |
cmd1  cmd2  "cmd3"
```

**AST Strategy:**

- **Node Types**: `AND` node with a `PIPE` on the left and a `COMMAND` on the right.
- **Structure**: The pipeline is executed first; if it succeeds, `cmd3` is executed.
- **Parsing Strategy**: Parse the pipeline, then recognize `&&` and create an `AND` node linking the pipeline and `cmd3`.
- **Execution Strategy**: Execute the pipeline; if it returns success, execute `cmd3`.

---

### 20. **Command with Logical OR and Pipes**

**Command Line:**

```bash
cmd1 || cmd2 | cmd3
```

**AST Diagram:**

```
      [OR]
     /    \
[COMMAND][PIPE]
   |      /    \
 "cmd1"[COMMAND][COMMAND]
            |        |
          "cmd2"   "cmd3"
```

**AST Strategy:**

- **Node Types**: `OR` node connecting a `COMMAND` and a `PIPE`.
- **Structure**: If `cmd1` fails, the pipeline `cmd2 | cmd3` is executed.
- **Parsing Strategy**: Recognize `||`, create an `OR` node, and parse the right side as a pipeline.
- **Execution Strategy**: Execute `cmd1`; if it fails, execute the pipeline.

---

### 21. **Command with Mixed Logical Operators**

**Command Line:**

```bash
cmd1 && cmd2 || cmd3 && cmd4
```

**AST Diagram:**

```
        [OR]
       /    \
    [AND]  [AND]
   /     \   /   \
cmd1   cmd2 cmd3 cmd4
```

**AST Strategy:**

- **Node Types**: Combination of `AND` and `OR` nodes.
- **Structure**: Logical operators are connected to represent the command's conditional execution flow.
- **Parsing Strategy**: Parse operators respecting their precedence (usually left-to-right in shells).
- **Execution Strategy**:
  - Execute `cmd1`; if it succeeds, execute `cmd2`.
  - If `cmd1 && cmd2` fails, execute `cmd3`; if `cmd3` succeeds, execute `cmd4`.

---

### 22. **Command with Multiple Pipes and Redirections**

**Command Line:**

```bash
cat file.txt | grep "pattern" | sort > result.txt
```

**AST Diagram:**

```
      [REDIRECT_OUT]
         /        \
      [PIPE]   "result.txt"
       /   \
   [PIPE][COMMAND]
   /   \       |
cmd1 cmd2   "sort"
   |    |
"cat""grep"
   |     |
"file.txt""pattern"
```

**AST Strategy:**

- **Node Types**: Nested `PIPE` nodes connected to a `REDIRECT_OUT` node.
- **Structure**: The output of the pipeline is redirected to `result.txt`.
- **Parsing Strategy**: Build the pipeline first, then apply output redirection.
- **Execution Strategy**: Execute the pipeline, redirecting the final output to `result.txt`.

---

### 23. **Command with Complex Redirections**

**Command Line:**

```bash
cmd < input.txt > output.txt 2> error.log
```

**AST Diagram:**

```
      [REDIRECT_ERR]
         /        \
  [REDIRECT_OUT]"error.log"
     /       \
[REDIRECT_IN]"output.txt"
    /      \
[COMMAND]"input.txt"
    |
  "cmd"
```

**AST Strategy:**

- **Node Types**: Chain of redirection nodes (`REDIRECT_IN`, `REDIRECT_OUT`, `REDIRECT_ERR`).
- **Structure**: Input is redirected from `input.txt`, output to `output.txt`, and errors to `error.log`.
- **Parsing Strategy**: Parse redirections in the order they appear, nesting nodes accordingly.
- **Execution Strategy**: Set up all redirections before executing `cmd`.

---

### 24. **Command with Here-Document and Pipe**

**Command Line:**

```bash
cat << EOF | grep "pattern"
Line 1
Line 2
EOF
```

**AST Diagram:**

```
     [PIPE]
    /     \
[HEREDOC][COMMAND]
   |         |
 "EOF"     "grep"
   |         |
[Here-doc]"pattern"
```

**AST Strategy:**

- **Node Types**: `HEREDOC` node connected to a `PIPE`.
- **Structure**: The here-document content is piped to `grep "pattern"`.
- **Parsing Strategy**: Build the `HEREDOC` node first, then create the `PIPE` node linking it to `grep`.
- **Execution Strategy**: Provide the here-document content to `cat`, pipe the output to `grep`.

---

### 25. **Command with Nested Subshells**

**Command Line:**

```bash
(echo "Start" && (ls || echo "Fallback"))
```

**AST Diagram:**

```
   [SUBSHELL]
       |
     [AND]
    /     \
[COMMAND][SUBSHELL]
   |          |
 "echo"     [OR]
   |        /    \
"Start" [COMMAND][COMMAND]
           |        |
         "ls"     "echo"
                    |
                "Fallback"
```

**AST Strategy:**

- **Node Types**: `SUBSHELL`, `AND`, `OR` nodes nested appropriately.
- **Structure**: Commands within parentheses are grouped under `SUBSHELL` nodes.
- **Parsing Strategy**: Upon encountering `(`, start a new `SUBSHELL`; parse enclosed commands recursively.
- **Execution Strategy**: Execute `echo "Start"`; if it succeeds, execute the inner subshell where `ls` is executed; if `ls` fails, execute `echo "Fallback"`.

---

### 26. **Command with Multiple Logical Operators and Redirections**

**Command Line:**

```bash
cmd1 && cmd2 > out.txt || cmd3 < in.txt
```

**AST Diagram:**

```
        [OR]
       /    \
    [REDIRECT_OUT][REDIRECT_IN]
       /       \       /     \
    [AND]   "out.txt"[COMMAND]"in.txt"
   /     \             |
cmd1   cmd2          cmd3
```

**AST Strategy:**

- **Node Types**: `AND` node connected to redirections and logical operators.
- **Structure**: The execution flow depends on the success or failure of commands, with redirections applied.
- **Parsing Strategy**: Parse `&&` and `||`, applying redirections to the appropriate commands.
- **Execution Strategy**:
  - Execute `cmd1`; if it succeeds, execute `cmd2` with output redirected to `out.txt`.
  - If `cmd1 && cmd2` fails, execute `cmd3` with input redirected from `in.txt`.

---

### 27. **Command with Wildcards and Pipes**

**Command Line:**

```bash
grep "main" *.c | sort
```

**AST Diagram:**

```
     [PIPE]
    /     \
[COMMAND][COMMAND]
   |         |
 "grep"    "sort"
   |
 "main"
   |
 "*.c"
```

**AST Strategy:**

- **Node Types**: `COMMAND` nodes connected by a `PIPE`.
- **Structure**: Wildcard expansion occurs before building the AST.
- **Parsing Strategy**: Expand `*.c` to match filenames, then build the AST with the expanded arguments.
- **Execution Strategy**: Execute `grep "main"` on all `.c` files, pipe the output to `sort`.

---

### 28. **Command with Background Execution and Redirection**

**Command Line:**

```bash
cmd &> output.log &
```

**AST Diagram:**

```
 [BACKGROUND]
      |
 [REDIRECT_ALL]
      /      \
 [COMMAND]"output.log"
     |
  "cmd"
```

**AST Strategy:**

- **Node Types**: `BACKGROUND` node wrapping a `REDIRECT_ALL` node.
- **Structure**: All output (stdout and stderr) is redirected to `output.log`, and the command runs in the background.
- **Parsing Strategy**: Recognize `&>` for redirecting all output, then `&` for background execution.
- **Execution Strategy**: Execute `cmd` in the background, redirecting all output to `output.log`.

---

### 29. **Exception Case: Invalid Syntax**

**Command Line:**

```bash
ls || && echo "Hello"
```

**AST Diagram:**

```
Syntax Error: Unexpected token '&&' after '||'
```

**AST Strategy:**

- **Error Detection**: The parser detects an invalid sequence of logical operators.
- **Parsing Strategy**: When `&&` follows `||` without an intervening command, raise a syntax error.
- **Execution Strategy**: Do not build an AST; report the error to the user.

---

### 30. **Exception Case: Multiple Input Redirections**

**Command Line:**

```bash
cmd < input1.txt < input2.txt
```

**AST Diagram:**

```
Syntax Error: Multiple input redirections are not allowed
```

**AST Strategy:**

- **Error Detection**: Shells typically allow only one input redirection.
- **Parsing Strategy**: Upon detecting a second `<`, check if an input redirection already exists; if so, raise an error.
- **Execution Strategy**: Report the syntax error and prevent execution.

---

### 31. **Exception Case: Misplaced Redirection**

**Command Line:**

```bash
> output.txt ls
```

**AST Diagram:**

```
Syntax Error: Redirection without command
```

**AST Strategy:**

- **Error Detection**: Redirection operators must be associated with a command.
- **Parsing Strategy**: If a redirection operator appears without a preceding command, raise an error.
- **Execution Strategy**: Inform the user of the syntax error.

---

### 32. **Command with Command Substitution** *(Note: Advanced Feature)*

**Command Line:**

```bash
echo $(date)
```

**AST Diagram:**

```
[COMMAND]
   |
 "echo"
   |
[SUBCOMMAND]
     |
 [COMMAND]
     |
  "date"
```

**AST Strategy:**

- **Note**: Command substitution is an advanced feature not required by the project.
- **Parsing Strategy**: Recognize `$(...)` and parse the enclosed command as a `SUBCOMMAND`.
- **Execution Strategy**: Execute the `date` command, capture its output, and provide it as an argument to `echo`.

---

### 33. **Command with Mixed Wildcards and Redirections**

**Command Line:**

```bash
cat *.txt > combined.txt
```

**AST Diagram:**

```
   [REDIRECT_OUT]
      /        \
 [COMMAND] "combined.txt"
     |
   "cat"
     |
  "*.txt"
```

**AST Strategy:**

- **Node Types**: `COMMAND` node with wildcard arguments, connected to a `REDIRECT_OUT` node.
- **Parsing Strategy**: Expand `*.txt` to a list of `.txt` files, then build the AST.
- **Execution Strategy**: Concatenate the contents of all `.txt` files and write the output to `combined.txt`.

---

### 34. **Command with Logical Operators and Subshell**

**Command Line:**

```bash
(cmd1 && cmd2) || cmd3
```

**AST Diagram:**

```
      [OR]
     /    \
 [SUBSHELL][COMMAND]
     |        |
   [AND]    "cmd3"
  /     \
cmd1   cmd2
```

**AST Strategy:**

- **Node Types**: `SUBSHELL`, `AND`, and `OR` nodes.
- **Structure**: The commands within parentheses are executed in a subshell.
- **Parsing Strategy**: Upon encountering `(`, create a `SUBSHELL` node; parse `cmd1 && cmd2` within it.
- **Execution Strategy**: Execute the subshell; if it fails, execute `cmd3`.

---

### 35. **Command with Pipes, Redirections, and Logical Operators**

**Command Line:**

```bash
cmd1 | cmd2 && cmd3 > out.txt || cmd4
```

**AST Diagram:**

```
          [OR]
         /    \
    [REDIRECT_OUT][COMMAND]
       /       \      |
     [AND]  "out.txt""cmd4"
    /     \
 [PIPE] [COMMAND]
  /   \      |
cmd1 cmd2  "cmd3"
```

**AST Strategy:**

- **Node Types**: `PIPE`, `AND`, `REDIRECT_OUT`, and `OR` nodes.
- **Structure**: The pipeline is evaluated first; if it succeeds, `cmd3` is executed with output redirection; otherwise, `cmd4` is executed.
- **Parsing Strategy**: Parse operators respecting their precedence, building the AST accordingly.
- **Execution Strategy**:
  - Execute `cmd1 | cmd2`; if it succeeds, execute `cmd3` redirecting output to `out.txt`.
  - If the pipeline or `cmd3` fails, execute `cmd4`.

---

### 36. **Command with Here-Document and Logical Operators**

**Command Line:**

```bash
cmd1 << EOF && cmd2
input
EOF
```

**AST Diagram:**

```
     [AND]
    /     \
[HEREDOC][COMMAND]
   |         |
 "EOF"     "cmd2"
   |
 "cmd1"
```

**AST Strategy:**

- **Node Types**: `HEREDOC` node connected to an `AND` node.
- **Structure**: `cmd1` receives input from the here-document; if it succeeds, `cmd2` is executed.
- **Parsing Strategy**: Parse the `<<` operator, build the `HEREDOC` node, then create the `AND` node upon encountering `&&`.
- **Execution Strategy**: Provide the here-document content to `cmd1`; if it succeeds, execute `cmd2`.

---

### 37. **Command with Multiple Here-Documents**

**Command Line:**

```bash
cmd1 << EOF1 | cmd2 << EOF2
input1
EOF1
input2
EOF2
```

**AST Diagram:**

```
     [PIPE]
    /     \
[HEREDOC][HEREDOC]
   |         |
 "EOF1"    "EOF2"
   |         |
"cmd1"    "cmd2"
```

**AST Strategy:**

- **Node Types**: Two `HEREDOC` nodes connected by a `PIPE`.
- **Structure**: Each command receives input from its respective here-document.
- **Parsing Strategy**: Parse each `<<` operator and build corresponding `HEREDOC` nodes, then connect them with a `PIPE`.
- **Execution Strategy**: Provide `input1` to `cmd1`, pipe its output to `cmd2` which receives `input2`.

---

### 38. **Command with Complex Subshells**

**Command Line:**

```bash
((cmd1 && cmd2) || cmd3) && cmd4
```

**AST Diagram:**

```
      [AND]
     /     \
 [SUBSHELL][COMMAND]
     |        |
    [OR]    "cmd4"
   /    \
[AND]  cmd3
 /   \
cmd1 cmd2
```

**AST Strategy:**

- **Node Types**: Nested `SUBSHELL`, `AND`, and `OR` nodes.
- **Structure**: The nested subshell groups the first set of commands.
- **Parsing Strategy**: Parse the innermost parentheses first, building the AST from the inside out.
- **Execution Strategy**:
  - Execute `cmd1 && cmd2` within the subshell; if it fails, execute `cmd3`.
  - If the subshell (result of `cmd1 && cmd2 || cmd3`) succeeds, execute `cmd4`.

---

### 39. **Exception Case: Unclosed Quotation**

**Command Line:**

```bash
echo "Unclosed string
```

**AST Diagram:**

```
Syntax Error: Unclosed quotation mark
```

**AST Strategy:**

- **Error Detection**: The parser detects that a quotation mark is not closed.
- **Parsing Strategy**: Keep track of quotation marks; if the end of input is reached without closure, raise an error.
- **Execution Strategy**: Do not build an AST; inform the user of the syntax error.

---

### 40. **Exception Case: Invalid Redirection Operator**

**Command Line:**

```bash
cmd1 >| output.txt
```

**AST Diagram:**

```
Syntax Error: Invalid redirection operator '>|'
```

**AST Strategy:**

- **Error Detection**: `>|` is not a standard redirection operator in this shell.
- **Parsing Strategy**: When encountering an unrecognized operator, raise a syntax error.
- **Execution Strategy**: Report the error and halt parsing.

---

### 41. **Command with Multiple Commands and Background Execution**

**Command Line:**

```bash
cmd1 & cmd2 && cmd3 &
```

**AST Diagram:**

```
[BACKGROUND]            [BACKGROUND]
     |                      |
 [COMMAND]              [AND]
     |                 /     \
  "cmd1"           [COMMAND][COMMAND]
                      |         |
                    "cmd2"    "cmd3"
```

**AST Strategy:**

- **Node Types**: `BACKGROUND`, `AND`, and `COMMAND` nodes.
- **Structure**: `cmd1` runs in the background; `cmd2 && cmd3` is executed, with `cmd3` running in the background.
- **Parsing Strategy**: Recognize `&` to create `BACKGROUND` nodes; parse `&&` to build `AND` nodes.
- **Execution Strategy**:
  - Execute `cmd1` in the background.
  - Execute `cmd2`; if it succeeds, execute `cmd3` in the background.

---

### 42. **Command with All Features Combined**

**Command Line:**

```bash
(cmd1 < in.txt | cmd2 && cmd3 > out.txt) || cmd4 & cmd5
```

**AST Diagram:**

```
          [OR]
         /    \
   [SUBSHELL][BACKGROUND]
       |             |
     [AND]        [COMMAND]
    /     \           |
 [PIPE] [REDIRECT_OUT]"cmd5"
  /   \        \
[REDIRECT_IN][COMMAND]
   /      \       |
[COMMAND]"in.txt""cmd3"
   |
 "cmd1"
      \
    "cmd2"
```

**AST Strategy:**

- **Node Types**: `SUBSHELL`, `PIPE`, `REDIRECT_IN`, `REDIRECT_OUT`, `AND`, `OR`, `BACKGROUND`.
- **Structure**: Complex nesting with multiple operators and redirections.
- **Parsing Strategy**:
  - Start with the subshell `( ... )`, parse inside commands.
  - Build the `PIPE` with input redirection for `cmd1`.
  - Connect `cmd3` with `&&` and apply output redirection.
  - The subshell result connects to `cmd4` via `||`.
  - `cmd5` runs in the background.
- **Execution Strategy**:
  - Execute the subshell:
    - Redirect input from `in.txt` to `cmd1`.
    - Pipe output to `cmd2`.
    - If the pipeline succeeds, execute `cmd3` with output redirected to `out.txt`.
  - If the subshell fails, execute `cmd4`.
  - Execute `cmd5` in the background.

---

### **A. Simple Commands and Arguments**

| Test Number       | Description                             | User Input                      | Expected Result                                                                                             |
|-------------------|-----------------------------------------|---------------------------------|-------------------------------------------------------------------------------------------------------------|
| **Test A1**       | Simple Command Execution                | `ls`                            | The shell lists the contents of the current directory.                                                      |
| **Test A2**       | Command with Arguments                  | `echo "Hello, World!"`          | The shell prints `Hello, World!` to the standard output.                                                    |
| **Test A3**       | Command with Multiple Arguments         | `cp file1.txt file2.txt`        | The shell copies `file1.txt` to `file2.txt`.                                                                |
| **Test A4**       | Command with Environment Variable       | `echo $HOME`                    | The shell prints the value of the `HOME` environment variable.                                              |
| **Test A5**       | Command with Quoted Arguments           | `mkdir "New Folder"`            | The shell creates a directory named `New Folder`.                                                           |
| **Test A6**       | Command with Single Quotes              | `echo 'Single quoted text'`     | The shell prints `Single quoted text`.                                                                      |
| **Test A7**       | Command with Escaped Characters         | `echo "This is a backslash: \\"`| The shell prints `This is a backslash: \`.                                                                  |
| **Test A8**       | Command with Special Characters         | `echo "Line1\nLine2"`           | The shell prints `Line1\nLine2` (interprets `\n` as characters, not newlines).                              |
| **Test A9**       | Command with Backticks (not supported)  | ``echo `date```                 | The shell treats backticks as normal characters, since command substitution is not required.                |
| **Test A10**      | Command with Semicolon Separator        | `echo "First command"; echo "Second command"` | The shell executes both commands sequentially.                                                              |
| **Exception A1**  | Unclosed Quotes                         | `echo "Unclosed string`         | The shell reports a syntax error: `Syntax Error: Unclosed quotation mark` and does not crash.               |
| **Exception A2**  | Invalid Command Syntax                  | `ech "Missing o"`               | The shell reports: `minishell: command not found: ech` and does not crash.                                  |

---

### **B. Input and Output Redirections**

| Test Number       | Description                            | User Input                                      | Expected Result                                                                                             |
|-------------------|----------------------------------------|-------------------------------------------------|-------------------------------------------------------------------------------------------------------------|
| **Test B1**       | Input Redirection                      | `sort < unsorted.txt`                           | The shell sorts the contents of `unsorted.txt` and outputs the sorted data to the standard output.          |
| **Test B2**       | Output Redirection                     | `ls > files.txt`                                | The shell writes the directory listing to `files.txt`.                                                      |
| **Test B3**       | Append Output Redirection              | `echo "New Entry" >> log.txt`                   | The shell appends `New Entry` to `log.txt`.                                                                 |
| **Test B4**       | Input and Output Redirection Combined  | `grep "search" < input.txt > output.txt`        | The shell reads from `input.txt`, searches for `"search"`, and writes the results to `output.txt`.          |
| **Test B5**       | Input Redirection with Missing File    | `cat < nonexistent.txt`                         | The shell reports an error: `minishell: nonexistent.txt: No such file or directory` and does not crash.     |
| **Test B6**       | Output Redirection with No Permission  | `echo "Test" > /root/output.txt`                | The shell reports an error: `minishell: /root/output.txt: Permission denied` and does not crash.            |
| **Test B7**       | Overwriting File with Output Redirection | `echo "Overwrite" > existing.txt`             | The shell overwrites `existing.txt` with `Overwrite`.                                                       |
| **Test B8**       | Multiple Output Redirections (Invalid) | `echo "Test" > out1.txt > out2.txt`             | The shell reports a syntax error: `Syntax Error: Multiple output redirections are not allowed` and does not crash. |
| **Test B9**       | Redirection with File Descriptor       | `ls 2> errors.txt`                              | The shell redirects standard error to `errors.txt`.                                                         |
| **Test B10**      | Here-String (Not Required)             | `cat <<< "Input String"`                        | The shell treats `<<<` as invalid syntax and reports an error: `Syntax Error: Invalid redirection operator` and does not crash. |
| **Exception B1**  | Multiple Input Redirections            | `cmd < input1.txt < input2.txt`                 | The shell reports a syntax error: `Syntax Error: Multiple input redirections are not allowed` and does not crash. |
| **Exception B2**  | Misplaced Redirection Operator         | `> output.txt ls`                               | The shell reports a syntax error: `Syntax Error: Redirection without command` and does not crash.           |

---

### **C. Pipes**

| Test Number       | Description                      | User Input                                    | Expected Result                                                                                             |
|-------------------|----------------------------------|-----------------------------------------------|-------------------------------------------------------------------------------------------------------------|
| **Test C1**       | Simple Pipe                      | `ls | grep "test"`                             | The shell lists files and pipes the output to `grep`, which filters and displays lines containing `"test"`. |
| **Test C2**       | Multiple Pipes                   | `cat file.txt | grep "error" | sort`        | The shell reads `file.txt`, filters lines containing `"error"`, sorts the results, and outputs them.        |
| **Test C3**       | Pipe with Redirection            | `ps aux | grep "python" > processes.txt`             | The shell writes the filtered process list to `processes.txt`.                                              |
| **Test C4**       | Pipe with Input Redirection      | `grep "search" < input.txt | sort`                   | The shell reads from `input.txt`, filters lines containing `"search"`, pipes to `sort`, and outputs results.|
| **Test C5**       | Pipe with Command Not Found      | `ls | nonexistentcommand`                        | The shell reports: `minishell: command not found: nonexistentcommand` and does not crash.                   |
| **Test C6**       | Chain of Pipes                   | `cmd1 | cmd2 | cmd3 | cmd4`                           | The shell executes a pipeline through all four commands.                                                    |
| **Test C7**       | Pipe with Built-in Command       | `echo "Test" | wc -c`                                    | The shell counts the number of characters in `"Test"` and outputs the result.                               |
| **Test C8**       | Pipe with Environment Variable   | `echo $PATH | tr ':' '\n'`                              | The shell splits the `PATH` variable by `:` and outputs each path on a new line.                            |
| **Test C9**       | Pipe with Background Execution   | `ls | grep "pattern" &`                          | The shell executes the pipeline in the background.                                                          |
| **Test C10**      | Pipe with Logical Operator       | `cmd1 | cmd2 && cmd3`                             | The shell executes `cmd1 | cmd2`; if it succeeds, executes `cmd3`.                                        |
| **Exception C1**  | Pipe with Missing Command        | `ls |`                                         | The shell reports a syntax error: `Syntax Error: Missing command after '|'` and does not crash.             |
| **Exception C2**  | Pipe Starting with Operator      | `| grep "pattern"`                           | The shell reports a syntax error: `Syntax Error: Unexpected '|' at the beginning of the command` and does not crash. |

---

### **D. Logical Operators**

| Test Number       | Description                      | User Input                            | Expected Result                                                                                                  |
|-------------------|----------------------------------|---------------------------------------|------------------------------------------------------------------------------------------------------------------|
| **Test D1**       | Logical AND Operator             | `make && make test`                   | The shell executes `make`; if it succeeds, it executes `make test`.                                              |
| **Test D2**       | Logical OR Operator              | `false || echo "Command failed"`      | Since `false` returns a non-zero status, the shell executes `echo "Command failed"`.                             |
| **Test D3**       | Combined Logical Operators       | `cmd1 && cmd2 || cmd3`                | If `cmd1` succeeds, execute `cmd2`; if `cmd2` fails, execute `cmd3`; if `cmd1` fails, execute `cmd3`.           |
| **Test D4**       | Multiple AND Operators           | `cmd1 && cmd2 && cmd3`                | The shell executes each command sequentially; if any command fails, subsequent commands are not executed.        |
| **Test D5**       | Multiple OR Operators            | `cmd1 || cmd2 || cmd3`                | The shell executes commands until one succeeds; once a command succeeds, it stops executing further commands.    |
| **Test D6**       | Logical Operators with Subshell  | `(cmd1 || cmd2) && cmd3`              | The shell executes the subshell; if it succeeds, executes `cmd3`.                                                |
| **Test D7**       | Logical Operators with Pipes     | `cmd1 | cmd2 && cmd3`                   | Executes the pipeline; if it succeeds, executes `cmd3`.                                                          |
| **Test D8**       | Logical Operators with Negation  | `! false && echo "Success"`           | Since `! false` is true, the shell executes `echo "Success"`.                                                    |
| **Test D9**       | Logical Operators with Non-zero Exit Codes | `false && echo "This will not print"` | Since `false` returns non-zero, the shell does not execute `echo`.                                               |
| **Test D10**      | Logical Operators with Built-ins | `export VAR="value" && echo $VAR`     | The shell sets `VAR` and then echoes its value.                                                                  |
| **Exception D1**  | Invalid Logical Operator Sequence| `ls || && echo "Hello"`               | The shell reports a syntax error: `Syntax Error: Unexpected token '&&' after '||'` and does not crash.          |
| **Exception D2**  | Logical Operator at Start        | `&& cmd`                              | The shell reports a syntax error: `Syntax Error: Unexpected '&&' at the beginning of the command` and does not crash. |

---

### **E. Subshells**

| Test Number       | Description           | User Input                           | Expected Result                                                                                             |
|-------------------|-----------------------|--------------------------------------|-------------------------------------------------------------------------------------------------------------|
| **Test E1**       | Simple Subshell       | `(echo "Start"; ls)`                 | The shell executes `echo "Start"` and `ls` in a subshell; any variable changes do not affect the parent shell. |
| **Test E2**       | Nested Subshells      | `(cmd1 && (cmd2 || cmd3))`           | The shell executes `cmd1`; if it succeeds, executes `cmd2`; if `cmd2` fails, executes `cmd3`, all within subshells. |
| **Test E3**       | Subshell with Redirection | `(ls > files.txt)`                | The shell executes `ls` and redirects output to `files.txt`, within a subshell.                             |
| **Test E4**       | Subshell with Variables | `(export VAR="value"; echo $VAR)`   | The shell sets `VAR` and echoes it within the subshell; `VAR` is not set in the parent shell.               |
| **Test E5**       | Subshell with Pipes   | `(cmd1 | cmd2) && cmd3`              | The shell executes the pipeline in a subshell; if it succeeds, executes `cmd3`.                             |
| **Test E6**       | Subshell with Background Execution | `(sleep 5; echo "Done") &` | The shell executes the subshell in the background.                                                          |
| **Test E7**       | Subshell with Logical Operators | `(false && echo "No") || echo "Yes"` | The subshell executes `false && echo "No"`; since it fails, the shell executes `echo "Yes"`.                |
| **Test E8**       | Empty Subshell        | `()`                                 | The shell does nothing and returns to the prompt without error.                                             |
| **Test E9**       | Subshell within Subshell| `((echo "Nested"))`                 | The shell executes the nested subshell and prints `Nested`.                                                 |
| **Test E10**      | Subshell with Command Not Found | `(nonexistentcommand)`         | The shell reports: `minishell: command not found: nonexistentcommand` within the subshell and does not crash. |
| **Exception E1**  | Unmatched Parentheses | `(echo "Start"`                      | The shell reports a syntax error: `Syntax Error: Unmatched '('` and does not crash.                         |
| **Exception E2**  | Misplaced Parentheses | `echo "Test")`                       | The shell reports a syntax error: `Syntax Error: Unexpected ')'` and does not crash.                        |

---

### **F. Here-Documents**

| Test Number       | Description                | User Input                                                                 | Expected Result                                                                                                 |
|-------------------|----------------------------|----------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------|
| **Test F1**       | Simple Here-Document       | `cat << EOF\nLine 1\nLine 2\nEOF`                                          | The shell passes the provided lines to `cat`, which outputs them.                                               |
| **Test F2**       | Here-Document with Variable| `cat << EOF\nValue is $VAR\nEOF`                                           | The shell does not expand `$VAR` in the here-document (unless specified), outputs `Value is $VAR`.              |
| **Test F3**       | Here-Document with Quotes  | `cat << 'EOF'\nValue is $VAR\nEOF`                                         | The shell treats the delimiter in single quotes; variables are not expanded, outputs `Value is $VAR`.           |
| **Test F4**       | Here-Document with Expansion| `cat << EOF\nValue is ${VAR}\nEOF`                                        | The shell outputs `Value is ${VAR}`; parameter expansion is not required unless specified.                      |
| **Test F5**       | Here-Document in Pipeline  | `grep "pattern" << EOF | sort\nLine 1\nLine 2\nEOF`                      | The shell passes the lines to `grep`, then pipes output to `sort`.                                              |
| **Test F6**       | Multiple Here-Documents    | `cmd1 << EOF1 | cmd2 << EOF2\nData1\nEOF1\nData2\nEOF2`             | The shell processes both here-documents and executes the pipeline.                                              |
| **Test F7**       | Here-Document with Empty Input | `cat << EOF\nEOF`                                                       | The shell passes empty input to `cat`, which outputs nothing.                                                   |
| **Test F8**       | Here-Document with Special Characters | `cat << EOF\nLine with * and ?\nEOF`                     | The shell treats `*` and `?` as normal characters in the here-document.                                         |
| **Test F9**       | Here-Document with Interrupt | `cat << EOF\nLine 1` *(User presses Ctrl+C)*                              | The shell interrupts the here-document input, reports `^C`, and returns to the prompt without crashing.         |
| **Test F10**      | Here-Document with Signal Handling | `cat << EOF\nLine 1` *(Send SIGTERM to shell)*                    | The shell handles the signal appropriately and exits gracefully.                                                |
| **Exception F1**  | Missing Here-Document Delimiter | `cat << EOF\nLine 1\nLine 2`                                           | The shell reports: `Syntax Error: Unexpected end of file, expected 'EOF'` and does not crash.                   |
| **Exception F2**  | Invalid Here-Document Delimiter | `cat << 123\nData\n123`                                              | The shell uses `123` as the delimiter; if numbers are invalid, reports syntax error: `Syntax Error: Invalid delimiter` (if applicable). |

---

### **G. Background Execution**

| Test Number       | Description                    | User Input                    | Expected Result                                                                                             |
|-------------------|--------------------------------|-------------------------------|-------------------------------------------------------------------------------------------------------------|
| **Test G1**       | Single Background Command      | `sleep 60 &`                  | The shell starts `sleep 60` in the background and returns to the prompt immediately.                        |
| **Test G2**       | Multiple Background Commands   | `cmd1 & cmd2 &`               | The shell starts both `cmd1` and `cmd2` in the background concurrently.                                     |
| **Test G3**       | Background Command with Output | `ls &> output.txt &`          | The shell runs `ls`, redirects all output to `output.txt`, and executes it in the background.               |
| **Test G4**       | Background Command with Pipes  | `cmd1 | cmd2 &`                | The shell executes the pipeline in the background.                                                          |
| **Test G5**       | Background Command with Logical Operators | `cmd1 && cmd2 &`    | The shell executes `cmd1`; if it succeeds, starts `cmd2` in the background.                                 |
| **Test G6**       | Background Command with Subshell | `(cmd1; cmd2) &`           | The shell executes the subshell in the background.                                                          |
| **Test G7**       | Background Command with Variable Assignment | `VAR=value &`     | The shell sets `VAR` in the background; however, it does not affect the parent shell's environment.         |
| **Test G8**       | Background Command with Job Control (Not Required) | `sleep 10 &`   | The shell starts `sleep 10` in the background; job control is not required, so minimal handling is acceptable. |
| **Test G9**       | Background Command with Redirection | `sleep 10 > sleep.log &`    | The shell redirects output of `sleep 10` to `sleep.log` and runs it in the background.                      |
| **Test G10**      | Background Command with Wait   | `sleep 5 & wait`              | The shell waits for background jobs to complete before returning to the prompt. (Note: `wait` is not required to be implemented.) |
| **Exception G1**  | Background Operator Without Command | `&`                         | The shell reports a syntax error: `Syntax Error: '&' without a preceding command` and does not crash.       |
| **Exception G2**  | Background Operator at Start   | `& cmd`                       | The shell reports a syntax error: `Syntax Error: Unexpected '&' at the beginning of the command` and does not crash. |

---

### **H. Wildcards and Globbing**

| Test Number       | Description                       | User Input                 | Expected Result                                                                                     |
|-------------------|-----------------------------------|----------------------------|-----------------------------------------------------------------------------------------------------|
| **Test H1**       | Simple Wildcard Expansion         | `ls *.c`                   | The shell expands `*.c` to match all `.c` files and lists them.                                     |
| **Test H2**       | Wildcards with Other Commands     | `grep "main" *.c`          | The shell searches for `"main"` in all `.c` files.                                                  |
| **Test H3**       | Wildcards Matching Directories    | `ls */`                    | The shell lists all directories in the current directory.                                           |
| **Test H4**       | Wildcards with Hidden Files       | `ls .*`                    | The shell lists all hidden files (those starting with `.`).                                         |
| **Test H5**       | Wildcards with Character Ranges   | `ls file[1-3].txt`         | The shell lists `file1.txt`, `file2.txt`, and `file3.txt` if they exist.                            |
| **Test H6**       | Wildcards with Question Mark      | `ls file?.txt`             | The shell lists files like `file1.txt`, `fileA.txt`, matching any single character in place of `?`. |
| **Test H7**       | Wildcards with Braces             | `ls {file1,file2}.txt`     | The shell expands to `ls file1.txt file2.txt` and lists those files.                                |
| **Test H8**       | Wildcards with Escape Characters  | `ls \*.c`                  | The shell treats `*.c` literally and tries to list a file named `*.c`.                              |
| **Test H9**       | Wildcards in Subdirectories       | `ls */*.c`                 | The shell lists all `.c` files in all subdirectories.                                               |
| **Test H10**      | Wildcards with No Matches         | `ls xyz*`                  | The shell reports: `ls: cannot access 'xyz*': No such file or directory` and does not crash.        |
| **Exception H1**  | No Matches for Wildcard           | `ls *.xyz`                 | The shell reports: `ls: cannot access '*.xyz': No such file or directory` and does not crash.       |
| **Exception H2**  | Invalid Wildcard Pattern          | `ls [a-`                   | The shell reports a syntax error: `Syntax Error: Invalid wildcard pattern` and does not crash.      |

---

### **I. Complex Commands**

| Test Number       | Description                        | User Input                                        | Expected Result                                                                                                     |
|-------------------|------------------------------------|---------------------------------------------------|---------------------------------------------------------------------------------------------------------------------|
| **Test I1**       | Combined Pipes and Redirections    | `cat file.txt | grep "pattern" | sort > result.txt`        | The shell processes the pipeline and writes the sorted output to `result.txt`.                                       |
| **Test I2**       | Mixed Logical Operators and Pipes  | `cmd1 | cmd2 && cmd3 || cmd4`                        | Execute `cmd1 | cmd2`; if it succeeds, execute `cmd3`; if it fails, execute `cmd4`.                                                 |
| **Test I3**       | Command with Subshell and Redirection | `(cmd1 && cmd2) > output.txt`                 | The shell executes the subshell; if successful, redirects output to `output.txt`.                                    |
| **Test I4**       | Command with Multiple Redirections | `cmd < input.txt > output.txt 2> error.log`      | The shell redirects input, output, and error streams appropriately.                                                 |
| **Test I5**       | Command with Here-Document and Pipe | `cmd1 << EOF | cmd2\nInput\nEOF`                    | The shell provides `Input` to `cmd1` and pipes the output to `cmd2`.                                                |
| **Test I6**       | Command with Background Execution and Logical Operators | `cmd1 & cmd2 && cmd3 &` | The shell runs `cmd1` in background; if `cmd2` succeeds, runs `cmd3` in background.                                 |
| **Test I7**       | Command with Wildcards and Pipes   | `grep "main" *.c | less`                              | The shell searches for `"main"` in all `.c` files and pipes the output to `less`.                                    |
| **Test I8**       | Command with Environment Variables and Subshell | `(export VAR="value"; cmd $VAR)`        | The shell sets `VAR` in subshell and executes `cmd $VAR`; `VAR` is not available outside the subshell.              |
| **Test I9**       | Command with Alias (Not Required)  | `alias ll='ls -l'; ll`                           | Since aliasing is not required, the shell treats `alias` as a command, and `ll` remains unrecognized.               |
| **Test I10**      | Command with Function Definition (Not Required) | `function greet() { echo "Hello"; }; greet` | Function definitions are not required; the shell treats `function` as a command and reports `command not found`.     |
| **Exception I1**  | Excessive Command Length           | `<very long command exceeding buffer limits>`     | The shell reports an error: `Error: Command exceeds maximum allowed length` and does not crash.                      |
| **Exception I2**  | Invalid Characters in Command      | `cmd\x01\x02`                                     | The shell reports a syntax error: `Syntax Error: Invalid character in command` and does not crash.                   |

---

### **J. Error Handling and Edge Cases**

| Test Number       | Description                      | User Input                       | Expected Result                                                                                             |
|-------------------|----------------------------------|----------------------------------|-------------------------------------------------------------------------------------------------------------|
| **Test J1**       | Command Not Found                | `nonexistentcommand`             | The shell reports: `minishell: command not found: nonexistentcommand` and does not crash.                   |
| **Test J2**       | Permission Denied                | `./script.sh`                    | If `script.sh` is not executable, the shell reports: `minishell: permission denied: ./script.sh` and does not crash. |
| **Test J3**       | Segmentation Fault Handling      | `./segfault_program`             | The shell reports: `Segmentation fault (core dumped)` and does not crash; it handles the signal properly.   |
| **Test J4**       | Keyboard Interrupt               | `sleep 100` *(Ctrl+C pressed)*   | The shell interrupts `sleep`, returns to the prompt, and does not crash.                                     |
| **Test J5**       | End-of-File (Ctrl+D) Handling    | *(Ctrl+D pressed at prompt)*     | The shell exits gracefully.                                                                                 |
| **Test J6**       | Unhandled Signal                 | *(Send `SIGTERM` to shell)*      | The shell exits gracefully, handling the signal appropriately.                                              |
| **Test J7**       | Division by Zero in Command      | `expr 1 / 0`                     | The shell reports an error: `expr: division by zero` and does not crash.                                    |
| **Test J8**       | File Not Found Error             | `cat nonexistentfile.txt`        | The shell reports: `minishell: nonexistentfile.txt: No such file or directory` and does not crash.          |
| **Test J9**       | Invalid Option Error             | `ls --invalidoption`             | The shell reports: `ls: unrecognized option '--invalidoption'` and displays usage information.              |
| **Test J10**      | Recursive Alias (Not Required)   | `alias ll='ll -l'; ll`           | Since aliasing is not required, the shell does not process aliases; reports `minishell: command not found: ll`. |
| **Exception J1**  | Command Exits with Non-zero Status| `false`                         | The shell recognizes the non-zero exit status; can be checked using `$?` if implemented.                    |
| **Exception J2**  | Infinite Loop Script             | `while true; do echo "Loop"; done` | Since scripting is not required, the shell treats `while` as a command and reports `command not found`.     |

---

### **K. Environment Variable Manipulation**

| Test Number       | Description                     | User Input                   | Expected Result                                                                                             |
|-------------------|---------------------------------|------------------------------|-------------------------------------------------------------------------------------------------------------|
| **Test K1**       | Setting Environment Variables   | `export VAR="value"`         | The shell sets `VAR` in the environment.                                                                    |
| **Test K2**       | Unsetting Environment Variables | `unset VAR`                  | The shell removes `VAR` from the environment.                                                               |
| **Test K3**       | Using Variables in Commands     | `echo $VAR`                  | The shell prints the value of `VAR`.                                                                        |
| **Test K4**       | Variable Persistence            | `export VAR="value"; exit; echo $VAR` | After restarting the shell, `VAR` is unset; the shell prints nothing.                                    |
| **Test K5**       | Listing All Environment Variables| `env`                        | The shell displays all environment variables.                                                               |
| **Test K6**       | Modifying PATH Variable         | `export PATH="/new/path:$PATH"; which cmd` | The shell updates `PATH` and uses it to locate commands.                               |
| **Test K7**       | Using Unset Variable            | `echo $UNSETVAR`             | The shell prints an empty line.                                                                             |
| **Test K8**       | Variable with No Value          | `export VAR; echo $VAR`      | The shell sets `VAR` with an empty value; `echo $VAR` outputs an empty line.                                |
| **Test K9**       | Variable Name with Underscore   | `export VAR_NAME="value"; echo $VAR_NAME` | The shell sets and prints `VAR_NAME`.                                              |
| **Test K10**      | Variable Overwriting            | `export VAR="first"; export VAR="second"; echo $VAR` | The shell updates `VAR` to `second` and prints `second`.                 |
| **Exception K1**  | Invalid Variable Name           | `export 1VAR="value"`        | The shell reports an error: `export: not an identifier: 1VAR` and does not crash.                           |
| **Exception K2**  | Unset Non-existent Variable     | `unset NONEXISTENT`          | The shell proceeds without error; nothing happens since the variable does not exist.                        |

---

### **L. Built-in Commands**

| Test Number       | Description                   | User Input         | Expected Result                                                                                             |
|-------------------|-------------------------------|--------------------|-------------------------------------------------------------------------------------------------------------|
| **Test L1**       | Change Directory to Absolute Path | `cd /tmp`      | The shell changes the current working directory to `/tmp`.                                                  |
| **Test L2**       | Change Directory to Relative Path | `cd ../`       | The shell changes the current working directory to the parent directory.                                    |
| **Test L3**       | Change Directory Without Arguments | `cd`          | The shell changes to the home directory (`$HOME`).                                                          |
| **Test L4**       | Print Working Directory       | `pwd`              | The shell prints the current working directory.                                                             |
| **Test L5**       | List Environment Variables    | `env`              | The shell displays all environment variables.                                                               |
| **Test L6**       | Echo with Options             | `echo -n "No Newline"` | The shell prints `No Newline` without a trailing newline.                                                   |
| **Test L7**       | Unset Built-in Command        | `unset PATH; ls`   | The shell unsets `PATH`, making external commands like `ls` unavailable unless absolute paths are used.     |
| **Test L8**       | Export Built-in Command       | `export VAR="value"; env | grep VAR` | The shell sets `VAR`, and `env` shows it in the environment variables.                  |
| **Test L9**       | Exit with Status              | `exit 42`          | The shell exits with status `42`.                                                                           |
| **Test L10**      | Exit with Multiple Arguments  | `exit 1 2 3`       | The shell reports an error: `exit: too many arguments` and does not exit.                                   |
| **Exception L1**  | Exit with Invalid Status      | `exit abc`         | The shell reports an error: `exit: numeric argument required` and exits with status `255`.                  |
| **Exception L2**  | Change to Non-existent Directory | `cd /nonexistent` | The shell reports an error: `cd: no such file or directory: /nonexistent` and does not crash.               |

---

### **M. Special Characters and Escaping**

| Test Number       | Description                       | User Input                                 | Expected Result                                                                                   |
|-------------------|-----------------------------------|--------------------------------------------|---------------------------------------------------------------------------------------------------|
| **Test M1**       | Escaped Characters                | `echo "This is a quote: \" "`              | The shell prints: `This is a quote: " `                                                           |
| **Test M2**       | Single vs. Double Quotes          | `echo '$HOME' "$HOME"`                     | The shell prints: `$HOME /home/user` (assuming `/home/user` is the value of `$HOME`).             |
| **Test M3**       | Escaping Spaces                   | `touch file\ name.txt; ls`                 | The shell creates a file named `file name.txt` and lists it.                                      |
| **Test M4**       | Escaping Special Characters       | `echo "\$PATH"`                            | The shell prints: `$PATH`                                                                         |
| **Test M5**       | Backslash at End of Line          | `echo "Line1\` *(Enter pressed)* `Line2"`  | The shell treats the backslash as a line continuation and prints: `Line1Line2`.                   |
| **Test M6**       | Quoting Wildcards                 | `ls "*.txt"`                               | The shell treats `*.txt` as a literal string and lists files named `*.txt` if any.                |
| **Test M7**       | Escaping in Single Quotes         | `echo 'It\'s a test'`                      | The shell treats backslashes as literal characters within single quotes and prints: `It\'s a test`.|
| **Test M8**       | Combining Quotes and Escapes      | `echo "Path: \$PATH"`                      | The shell prints: `Path: $PATH` (the `$PATH` variable is not expanded due to the backslash).      |
| **Test M9**       | Escaping Backslash                | `echo "\\\\"`                              | The shell prints: `\\`                                                                            |
| **Test M10**      | Command Substitution Characters   | `echo "Today is $(date)"`                  | Since command substitution is not required, the shell prints: `Today is $(date)`.                 |
| **Exception M1**  | Invalid Escape Sequence           | `echo \z`                                  | The shell prints: `z` and does not crash.                                                         |
| **Exception M2**  | Unmatched Quotes                  | `echo "Unmatched quote`                    | The shell reports a syntax error: `Syntax Error: Unclosed quotation mark` and does not crash.     |

---

### **N. Scripting and Execution**

| Test Number       | Description                        | User Input                                | Expected Result                                                                                             |
|-------------------|------------------------------------|-------------------------------------------|-------------------------------------------------------------------------------------------------------------|
| **Test N1**       | Execute Script File                | `./script.sh`                             | The shell executes `script.sh` if it has execute permissions and is properly formatted.                     |
| **Test N2**       | Shebang Execution                  | `#!/bin/minishell\necho "Hello from script"` | The shell executes the script using the `minishell` interpreter.                                          |
| **Test N3**       | Script with Arguments              | `./script.sh arg1 arg2`                   | The shell passes `arg1` and `arg2` to `script.sh` as arguments.                                             |
| **Test N4**       | Script without Execute Permission  | `./script.sh`                             | The shell reports: `minishell: permission denied: ./script.sh` and does not crash.                          |
| **Test N5**       | Source Command (Not Required)      | `source script.sh`                        | Since `source` is not required, the shell treats it as a command and reports `command not found`.           |
| **Test N6**       | Execute Binary File                | `./binaryfile`                            | If `binaryfile` is an executable binary, the shell executes it.                                             |
| **Test N7**       | Execute Script with Invalid Shebang| `#!/bin/invalidshell\necho "Test"`        | The shell reports: `minishell: bad interpreter: No such file or directory` and does not crash.              |
| **Test N8**       | Script with Environment Variables  | `export VAR="value"; ./script.sh`         | The shell passes `VAR` to `script.sh` if environment variables are inherited.                               |
| **Test N9**       | Execute Script from Relative Path  | `cd scripts; ./script.sh`                 | The shell changes directory and executes `script.sh` in the `scripts` directory.                            |
| **Test N10**      | Execute Script with Input Redirection | `./script.sh < input.txt`              | The shell provides `input.txt` as standard input to `script.sh`.                                             |
| **Exception N1**  | Recursive Shell Execution          | `#!/bin/minishell\n./script.sh`           | The shell should detect recursion or reach maximum process limits, and handle it gracefully without crashing the system. |
| **Exception N2**  | Execute Non-existent Script        | `./nonexistent.sh`                        | The shell reports: `minishell: ./nonexistent.sh: No such file or directory` and does not crash.             |

