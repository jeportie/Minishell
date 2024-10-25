/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:45:53 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/25 14:03:39 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "minishell.h"
# include "tokenize.h"

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_REDIRECT_APPEND,
	NODE_REDIRECT_HEREDOC,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}			t_node_type;

typedef struct s_cmd_node
{
	char	**argv;
	int		argc;
}				t_cmd_node;

typedef struct s_pipe_node
{
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}				t_pipe_node;

typedef struct s_redirect_node
{
	t_node_type			type;
	struct s_ast_node	*child;
	char				*filename;
}				t_redirect_node;

typedef struct s_heredoc_node
{
	struct s_ast_node	*child;
	char				*delimiter;
}				t_heredoc_node;

typedef struct s_logic_node
{
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}				t_logic_node;

typedef struct s_subshell_node
{
	struct s_ast_node	*child;
}				t_subshell_node;

typedef union u_node_data
{
	t_cmd_node			command;
	t_pipe_node			pipe;
	t_redirect_node		redirect;
	t_heredoc_node		heredoc;
	t_logic_node		logic;
	t_subshell_node		subshell;
}						t_node_data;

typedef struct s_ast_node
{
	t_node_type	type;
	t_node_data	data;
}				t_ast_node;

/* Main Function */
t_ast_node	*ms_parse_tokens(t_token *tokens, t_gc *gcl);

/* Parsing functions */
t_ast_node	*parse_sequence(t_token **current_token, t_gc *gcl);
t_ast_node	*parse_logical(t_token **current_token, t_gc *gcl);
t_ast_node	*parse_pipeline(t_token **current_token, t_gc *gcl);
t_ast_node	*parse_command(t_token **current_token, t_gc *gcl);
t_ast_node	*parse_redirection(t_token **current_token,
				t_ast_node *child, t_gc *gcl);
t_ast_node	*parse_subshell(t_token **current_token, t_gc *gcl);

/* Node Functions*/
t_ast_node	*create_command_node(t_token **current_token, t_gc *gcl);
t_ast_node	*create_pipe_node(t_ast_node *left, t_ast_node *right, t_gc *gcl);
t_ast_node	*create_redirect_node(t_node_type type, t_ast_node *child,
				char *filename, t_gc *gcl);
t_ast_node	*create_heredoc_node(t_ast_node *child, char *delimiter, t_gc *gcl);
t_ast_node	*create_logical_node(t_node_type type, t_ast_node *left,
				t_ast_node *right, t_gc *gcl);
t_ast_node	*create_subshell_node(t_ast_node *child, t_gc *gcl);

/* Utils Functions */
bool		is_pipe_op(t_token *current_token);
bool		is_logical_op(t_token *current_token);
void		print_ast(t_ast_node *node, int depth, char *prefix, int is_left);
void		expand_wildcards(t_ast_node *ast, t_gc *gcl);

#endif /*AST_H*/

/*
```
echo "Hello" && ls | grep "file" > output.txt
```
---

## **Step 1: Tokenization**

First, the command line input is tokenized into a list of tokens.

**Tokens Generated:**

1. `TOKEN_WORD`: `"echo"`
2. `TOKEN_WORD`: `"Hello"`
3. `TOKEN_AND`: `"&&"`
4. `TOKEN_WORD`: `"ls"`
5. `TOKEN_PIPE`: `"|"`
6. `TOKEN_WORD`: `"grep"`
7. `TOKEN_WORD`: `"file"`
8. `TOKEN_REDIRECTION`: `">"`
9. `TOKEN_WORD`: `"output.txt"`

**Visual Representation of Tokens:**

```
[1] TOKEN_WORD: "echo"
[2] TOKEN_WORD: "Hello"
[3] TOKEN_AND: "&&"
[4] TOKEN_WORD: "ls"
[5] TOKEN_PIPE: "|"
[6] TOKEN_WORD: "grep"
[7] TOKEN_WORD: "file"
[8] TOKEN_REDIRECTION: ">"
[9] TOKEN_WORD: "output.txt"
```

---

## **Step 2: Parsing Begins**

**Function Call:**

```c
t_ast_node *ms_parse_tokens(t_token *tokens, t_gc *gcl);
```

- **Initial Variables:**

  - `current_token`: Points to the first token (`[1] TOKEN_WORD: "echo"`).

---

## **Step 3: Parsing Sequence**

**Function Call:**

```c
t_ast_node *parse_sequence(t_token **current_token, t_gc *gcl);
```

- **Input:**

  - `*current_token`: `[1] TOKEN_WORD: "echo"`

- **Process:**

  - Call `parse_logical(current_token, gcl);`

---

## **Step 4: Parsing Logical Expression**

**Function Call:**

```c
t_ast_node *parse_logical(t_token **current_token, t_gc *gcl);
```

- **Input:**

  - `*current_token`: `[1] TOKEN_WORD: "echo"`

- **Process:**

  - Call `parse_pipeline(current_token, gcl);`

---

## **Step 5: Parsing Pipeline**

**Function Call:**

```c
t_ast_node *parse_pipeline(t_token **current_token, t_gc *gcl);
```

- **Input:**

  - `*current_token`: `[1] TOKEN_WORD: "echo"`

- **Process:**

  - Call `parse_command(current_token, gcl);`

---

## **Step 6: Parsing Command ("echo 'Hello'")**

**Function Call:**

```c
t_ast_node *parse_command(t_token **current_token, t_gc *gcl);
```

- **Initial Variables:**

  - `*current_token`: `[1] TOKEN_WORD: "echo"`
  - `command_node`: `NULL`

- **Process:**

  1. **Create Command Node:**

     **Function Call:**

     ```c
     t_ast_node *create_command_node(t_token **current_token, t_gc *gcl);
     ```

     - **Input:**

       - `*current_token`: `[1] TOKEN_WORD: "echo"`
       - `argc`: `0`
       - `argv`: `NULL`

     - **Process:**

       - Initialize an empty `argv` array.

       - **Loop to Collect Arguments:**

         - **Iteration 1:**

           - `*current_token`: `[1] TOKEN_WORD: "echo"`
           - Token is `TOKEN_WORD`.

           - Add `"echo"` to `argv`.
           - Increment `argc` to `1`.

           - Advance `*current_token` to `[2] TOKEN_WORD: "Hello"`

         - **Iteration 2:**

           - `*current_token`: `[2] TOKEN_WORD: "Hello"`
           - Token is `TOKEN_WORD`.

           - Add `"Hello"` to `argv`.
           - Increment `argc` to `2`.

           - Advance `*current_token` to `[3] TOKEN_AND: "&&"`

         - **Iteration 3:**

           - `*current_token`: `[3] TOKEN_AND: "&&"`
           - Token is not `TOKEN_WORD`.

           - Exit the loop.

       - **Final `argv` and `argc`:**

         - `argv`: `["echo", "Hello"]`
         - `argc`: `2`

     - **Create `t_cmd_node`:**

       - `t_cmd_node`:

         - `argv`: `["echo", "Hello"]`
         - `argc`: `2`

     - **Create `t_ast_node` of type `NODE_COMMAND`:**

       - `command_node`:

         - `type`: `NODE_COMMAND`
         - `data.command`: The `t_cmd_node` created above.

     - **Return `command_node`**

     - **Variables After Function:**

       - `command_node`: The node representing `echo "Hello"`
       - `*current_token`: `[3] TOKEN_AND: "&&"`

  2. **Check for Redirections:**

     - No redirection tokens (`<`, `>`, `>>`, `<<`) at `*current_token`.

  3. **Check for Background Execution:**

     - No background execution token (`&`) at `*current_token`.

- **Return `command_node`**

- **Variables After Function:**

  - `command_node`: The node representing `echo "Hello"`
  - `*current_token`: `[3] TOKEN_AND: "&&"`

---

## **Step 7: Back to Parsing Pipeline**

- **Variables:**

  - `left_node`: The node representing `echo "Hello"`
  - `*current_token`: `[3] TOKEN_AND: "&&"`

- **Process:**

  - Check for `TOKEN_PIPE` (`|`) at `*current_token`.

  - No pipe detected.

- **Return `left_node`**

- **Variables After Function:**

  - `left_node`: The node representing `echo "Hello"`
  - `*current_token`: `[3] TOKEN_AND: "&&"`

---

## **Step 8: Back to Parsing Logical Expression**

- **Variables:**

  - `left_node`: The node representing `echo "Hello"`
  - `*current_token`: `[3] TOKEN_AND: "&&"`

- **Process:**

  - Detect `TOKEN_AND` (`&&`) at `*current_token`.

  - **Operator Type:**

    - `op_type`: `NODE_AND`

  - Advance `*current_token` to `[4] TOKEN_WORD: "ls"`

- **Parse Right Side of `&&`**

  - Call `parse_pipeline(current_token, gcl);`

---

## **Step 9: Parsing Pipeline (After `&&`)**

**Function Call:**

```c
t_ast_node *parse_pipeline(t_token **current_token, t_gc *gcl);
```

- **Input:**

  - `*current_token`: `[4] TOKEN_WORD: "ls"`

- **Process:**

  - Call `parse_command(current_token, gcl);`

---

## **Step 10: Parsing Command ("ls")**

**Function Call:**

```c
t_ast_node *parse_command(t_token **current_token, t_gc *gcl);
```

- **Initial Variables:**

  - `*current_token`: `[4] TOKEN_WORD: "ls"`
  - `command_node`: `NULL`

- **Process:**

  1. **Create Command Node:**

     **Function Call:**

     ```c
     t_ast_node *create_command_node(t_token **current_token, t_gc *gcl);
     ```

     - **Input:**

       - `*current_token`: `[4] TOKEN_WORD: "ls"`
       - `argc`: `0`
       - `argv`: `NULL`

     - **Process:**

       - Initialize an empty `argv` array.

       - **Loop to Collect Arguments:**

         - **Iteration 1:**

           - `*current_token`: `[4] TOKEN_WORD: "ls"`
           - Token is `TOKEN_WORD`.

           - Add `"ls"` to `argv`.
           - Increment `argc` to `1`.

           - Advance `*current_token` to `[5] TOKEN_PIPE: "|"`

         - **Iteration 2:**

           - `*current_token`: `[5] TOKEN_PIPE: "|"`
           - Token is not `TOKEN_WORD`.

           - Exit the loop.

       - **Final `argv` and `argc`:**

         - `argv`: `["ls"]`
         - `argc`: `1`

     - **Create `t_cmd_node`:**

       - `t_cmd_node`:

         - `argv`: `["ls"]`
         - `argc`: `1`

     - **Create `t_ast_node` of type `NODE_COMMAND`:**

       - `command_node`:

         - `type`: `NODE_COMMAND`
         - `data.command`: The `t_cmd_node` created above.

     - **Return `command_node`**

     - **Variables After Function:**

       - `command_node`: The node representing `ls`
       - `*current_token`: `[5] TOKEN_PIPE: "|"`

  2. **Check for Redirections:**

     - No redirection tokens at `*current_token`.

  3. **Check for Background Execution:**

     - No background execution token at `*current_token`.

- **Return `command_node`**

- **Variables After Function:**

  - `command_node`: The node representing `ls`
  - `*current_token`: `[5] TOKEN_PIPE: "|"`

---

## **Step 11: Back to Parsing Pipeline (After `&&`)**

- **Variables:**

  - `left_node`: The node representing `ls`
  - `*current_token`: `[5] TOKEN_PIPE: "|"`

- **Process:**

  - Detect `TOKEN_PIPE` (`|`) at `*current_token`.

  - Advance `*current_token` to `[6] TOKEN_WORD: "grep"`

- **Parse Right Side of `|`**

  - Call `parse_command(current_token, gcl);`

---

## **Step 12: Parsing Command ("grep 'file'")**

**Function Call:**

```c
t_ast_node *parse_command(t_token **current_token, t_gc *gcl);
```

- **Initial Variables:**

  - `*current_token`: `[6] TOKEN_WORD: "grep"`
  - `command_node`: `NULL`

- **Process:**

  1. **Create Command Node:**

     **Function Call:**

     ```c
     t_ast_node *create_command_node(t_token **current_token, t_gc *gcl);
     ```

     - **Input:**

       - `*current_token`: `[6] TOKEN_WORD: "grep"`
       - `argc`: `0`
       - `argv`: `NULL`

     - **Process:**

       - Initialize an empty `argv` array.

       - **Loop to Collect Arguments:**

         - **Iteration 1:**

           - `*current_token`: `[6] TOKEN_WORD: "grep"`
           - Token is `TOKEN_WORD`.

           - Add `"grep"` to `argv`.
           - Increment `argc` to `1`.

           - Advance `*current_token` to `[7] TOKEN_WORD: "file"`

         - **Iteration 2:**

           - `*current_token`: `[7] TOKEN_WORD: "file"`
           - Token is `TOKEN_WORD`.

           - Add `"file"` to `argv`.
           - Increment `argc` to `2`.

           - Advance `*current_token` to `[8] TOKEN_REDIRECTION: ">"`

         - **Iteration 3:**

           - `*current_token`: `[8] TOKEN_REDIRECTION: ">"`
           - Token is not `TOKEN_WORD`.

           - Exit the loop.

       - **Final `argv` and `argc`:**

         - `argv`: `["grep", "file"]`
         - `argc`: `2`

     - **Create `t_cmd_node`:**

       - `t_cmd_node`:

         - `argv`: `["grep", "file"]`
         - `argc`: `2`

     - **Create `t_ast_node` of type `NODE_COMMAND`:**

       - `command_node`:

         - `type`: `NODE_COMMAND`
         - `data.command`: The `t_cmd_node` created above.

     - **Return `command_node`**

     - **Variables After Function:**

       - `command_node`: The node representing `grep "file"`
       - `*current_token`: `[8] TOKEN_REDIRECTION: ">"`

  2. **Check for Redirections:**

     - Detect redirection token at `*current_token`.

     - **Loop to Parse Redirections:**

       - **First Iteration:**

         - **Redirection Token:** `[8] TOKEN_REDIRECTION: ">"`

         - **Function Call:**

           ```c
           t_ast_node *parse_redirection(t_token **current_token, t_ast_node *child, t_gc *gcl);
           ```

         - **Input:**

           - `*current_token`: `[8] TOKEN_REDIRECTION: ">"`
           - `child`: The node representing `grep "file"`

         - **Process:**

           - Determine redirection type:

             - `redir_type`: `NODE_REDIRECT_OUT`

           - Advance `*current_token` to `[9] TOKEN_WORD: "output.txt"`

           - **Check for Filename:**

             - `*current_token`: `[9] TOKEN_WORD: "output.txt"`

           - **Filename:** `"output.txt"`

           - Advance `*current_token` to `NULL` (end of tokens)

           - **Create Redirect Node:**

             **Function Call:**

             ```c
             t_ast_node *create_redirect_node(t_node_type type, t_ast_node *child, char *filename, t_gc *gcl);
             ```

             - **Input:**

               - `type`: `NODE_REDIRECT_OUT`
               - `child`: The node representing `grep "file"`
               - `filename`: `"output.txt"`

             - **Process:**

               - Create `t_redirect_node`:

                 - `type`: `NODE_REDIRECT_OUT`
                 - `child`: The node representing `grep "file"`
                 - `filename`: `"output.txt"`

               - Create `t_ast_node` of type `NODE_REDIRECT_OUT`:

                 - `type`: `NODE_REDIRECT_OUT`
                 - `data.redirect`: The `t_redirect_node` created above.

             - **Return Redirect Node**

           - **Return Redirect Node**

         - **Variables After Function:**

           - `command_node`: The redirection node representing `> output.txt` applied to `grep "file"`
           - `*current_token`: `NULL`

       - **End of Redirection Parsing Loop**

  3. **Check for Background Execution:**

     - No background execution token at `*current_token`.

- **Return `command_node`**

- **Variables After Function:**

  - `command_node`: The redirection node representing `> output.txt` applied to `grep "file"`
  - `*current_token`: `NULL`

---

## **Step 13: Back to Parsing Pipeline (After `|`)**

- **Variables:**

  - `right_node`: The redirection node representing `grep "file" > output.txt`
  - `*current_token`: `NULL`

- **Process:**

  - Create Pipe Node:

    **Function Call:**

    ```c
    t_ast_node *create_pipe_node(t_ast_node *left, t_ast_node *right, t_gc *gcl);
    ```

    - **Input:**

      - `left`: The node representing `ls`
      - `right`: The node representing `grep "file" > output.txt`

    - **Process:**

      - Create `t_pipe_node`:

        - `left`: The node representing `ls`
        - `right`: The node representing `grep "file" > output.txt`

      - Create `t_ast_node` of type `NODE_PIPE`:

        - `type`: `NODE_PIPE`
        - `data.pipe`: The `t_pipe_node` created above.

    - **Return Pipe Node**

  - Update `left_node`:

    - `left_node`: The pipe node representing `ls | grep "file" > output.txt`

- **Variables After Function:**

  - `left_node`: The pipe node representing `ls | grep "file" > output.txt`
  - `*current_token`: `NULL`

---

## **Step 14: Back to Parsing Logical Expression (After `&&`)**

- **Variables:**

  - `right_node`: The pipe node representing `ls | grep "file" > output.txt`
  - `op_type`: `NODE_AND`
  - `left_node`: The node representing `echo "Hello"`

- **Process:**

  - Create Logical Node:

    **Function Call:**

    ```c
    t_ast_node *create_logical_node(t_node_type type, t_ast_node *left, t_ast_node *right, t_gc *gcl);
    ```

    - **Input:**

      - `type`: `NODE_AND`
      - `left`: The node representing `echo "Hello"`
      - `right`: The pipe node representing `ls | grep "file" > output.txt`

    - **Process:**

      - Create `t_logic_node`:

        - `left`: The node representing `echo "Hello"`
        - `right`: The pipe node representing `ls | grep "file" > output.txt`

      - Create `t_ast_node` of type `NODE_AND`:

        - `type`: `NODE_AND`
        - `data.logic`: The `t_logic_node` created above.

    - **Return Logical Node**

  - Update `left_node`:

    - `left_node`: The logical node representing `echo "Hello" && (ls | grep "file" > output.txt)`

- **Variables After Function:**

  - `left_node`: The logical node representing `echo "Hello" && (ls | grep "file" > output.txt)`
  - `*current_token`: `NULL`

---

## **Step 15: Back to Parsing Sequence**

- **Variables:**

  - `left_node`: The logical node representing the entire command
  - `*current_token`: `NULL`

- **Process:**

  - No more sequence separators (`;`) detected.

- **Return `left_node`**

- **Variables After Function:**

  - `ast_root`: The logical node representing the entire command

---

## **Step 16: Back to `ms_parse_tokens`**

- **Variables:**

  - `ast_root`: The logical node representing the entire command
  - `current_token`: `NULL`

- **Process:**

  - Check if `current_token` is `NULL`.

    - It is `NULL`, so parsing is complete.

- **Return `ast_root`**

---

## **Final AST Structure**

**AST Root:**

- **Type**: `NODE_AND`
- **Data**: `t_logic_node`
  - **Left**: `NODE_COMMAND` representing `echo "Hello"`
  - **Right**: `NODE_PIPE`
    - **Left**: `NODE_COMMAND` representing `ls`
    - **Right**: `NODE_REDIRECT_OUT`
      - **Filename**: `"output.txt"`
      - **Child**: `NODE_COMMAND` representing `grep "file"`

**Visual Representation:**

```
           [NODE_AND]
           /       \
       [CMD]       [NODE_PIPE]
     ("echo",      /         \
      "Hello")  [CMD]     [NODE_REDIRECT_OUT]
                "ls"         |
                           [CMD]
                       ("grep", "file")
                       Output: "output.txt"
```

---

## **Summary of Variable Changes**

- **`current_token`** advanced through each token, pointing to:

  1. `[1] TOKEN_WORD: "echo"`
  2. `[2] TOKEN_WORD: "Hello"`
  3. `[3] TOKEN_AND: "&&"`
  4. `[4] TOKEN_WORD: "ls"`
  5. `[5] TOKEN_PIPE: "|"`
  6. `[6] TOKEN_WORD: "grep"`
  7. `[7] TOKEN_WORD: "file"`
  8. `[8] TOKEN_REDIRECTION: ">"`
  9. `[9] TOKEN_WORD: "output.txt"`
  10. `NULL` (end of tokens)

- **`left_node`** and **`right_node`** were updated at each level to build the AST nodes.

- **AST Nodes Created:**

  - `NODE_COMMAND`: `"echo", "Hello"`
  - `NODE_COMMAND`: `"ls"`
  - `NODE_COMMAND`: `"grep", "file"`
  - `NODE_REDIRECT_OUT`: Applied to `grep "file"`
  - `NODE_PIPE`: Between `ls` and `grep "file" > output.txt`
  - `NODE_AND`: Between `echo "Hello"` and `ls | grep "file" > output.txt`

---
*/
