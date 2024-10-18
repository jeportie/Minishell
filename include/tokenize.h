/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:01:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/18 12:58:02 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OPERATOR,
	TOKEN_REDIRECTION,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SEQUENCE,
	TOKEN_EXEC,
	TOKEN_BACKGROUND,
	TOKEN_SUBSHELL_START,
	TOKEN_SUBSHELL_STOP,
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*token;
	struct s_token	*next;
}			t_token;

/* Main Function */
t_token	*ms_tokenize(const char *commant_line, t_gc *gcl);

/* Token List Functions */
t_token	*create_token(t_token_type type, const char *value, t_gc *gcl);
void	add_token(t_token **head, t_token *new_token);

/* Utils functions*/
bool	is_whitespace(char c);
bool	is_operator(char c);
bool	is_quote(char c);
bool	is_special(char c);
void	skip_whitespace(const char **input);
void	print_token(t_token *head);

/* Tokenize Functions */
t_token	*tokenize_word(const char **input, t_gc *gcl);
t_token	*tokenize_quote(const char **input, t_gc *gcl);

t_token	*tokenize_operator(const char **input, t_gc *gcl);
t_token	*map_token_type_to_operator(const char *operator_token);

#endif /*TOKENIZE_H*/

/*
## **Step-by-Step Tokenization Process**

We'll go through several examples, explaining how the tokenizer processes each character and builds the token list. We'll track variable values and data flow.

### **Example 1: Simple Command**

**Command Line Input:**

```
ls -l /home
```

**Expected Tokens:**

1. `TOKEN_WORD`: `"ls"`
2. `TOKEN_WORD`: `"-l"`
3. `TOKEN_WORD`: `"/home"`

---

### **Detailed Tokenization Steps**

#### **1. Initial Setup**

- **Function Call:**

  ```c
  t_token *ms_tokenize(const char *command_line, t_gc *gcl);
  ```

- **Variables:**

  - `command_line`: `"ls -l /home"`
  - `input`: Pointer to the first character of `command_line`.
  - `tokens`: `NULL` (the head of the token list).
  - `new_token`: `NULL` (used to store newly created tokens).

---

#### **2. First Iteration**

- **Current Input Position:**

  - `*input`: `'l'` (first character of `"ls -l /home"`)

- **Process:**

  - **Skip Whitespace:**

    - Call `skip_whitespace(&input);`
    - No leading whitespace; `input` remains at `'l'`.

  - **Determine Token Type:**

    - Since `is_quote(*input)` is `false` and `is_operator(*input)` is `false`, we proceed to `tokenize_word`.

  - **Tokenize Word:**

    - **Function Call:**

      ```c
      t_token *tokenize_word(const char **input, t_gc *gcl);
      ```

    - **Variables:**

      - `start`: Pointer to the current position of `*input` (`'l'`).
      - `value`: Empty string buffer.

    - **Process:**

      - **Collect Characters Until Whitespace or Operator:**

        - **Character `'l'`:**

          - Not whitespace, operator, or special.
          - Append `'l'` to `value`: `value = "l"`
          - Advance `*input` to `'s'`.

        - **Character `'s'`:**

          - Not whitespace, operator, or special.
          - Append `'s'` to `value`: `value = "ls"`
          - Advance `*input` to `' '` (space).

        - **Character `' '`:**

          - Whitespace detected.
          - Exit the loop.

      - **Create Token:**

        - **Function Call:**

          ```c
          t_token *create_token(TOKEN_WORD, "ls", gcl);
          ```

        - **Token Created:**

          - `type`: `TOKEN_WORD`
          - `token`: `"ls"`
          - `next`: `NULL`

    - **Return Token:**

      - `new_token`: Pointer to the token created for `"ls"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

  - **Process:**

    - Since `tokens` is `NULL`, `tokens` now points to `new_token`.

- **Variables After First Iteration:**

  - `tokens`: Head of the token list (`"ls"`)
  - `input`: Points to `' '` (space)

---

#### **3. Second Iteration**

- **Skip Whitespace:**

  - Call `skip_whitespace(&input);`
  - Skip the space; `input` now points to `'-'`.

- **Determine Token Type:**

  - Since `is_quote(*input)` is `false` and `is_operator(*input)` is `false`, we proceed to `tokenize_word`.

- **Tokenize Word:**

  - **Function Call:**

    ```c
    t_token *tokenize_word(const char **input, t_gc *gcl);
    ```

  - **Variables:**

    - `start`: Pointer to `'-'`
    - `value`: Empty string buffer.

  - **Process:**

    - **Collect Characters Until Whitespace or Operator:**

      - **Character `'-'`:**

        - Not whitespace, operator, or special.
        - Append `'-'` to `value`: `value = "-"`
        - Advance `*input` to `'l'`.

      - **Character `'l'`:**

        - Not whitespace, operator, or special.
        - Append `'l'` to `value`: `value = "-l"`
        - Advance `*input` to `' '` (space).

      - **Character `' '`:**

        - Whitespace detected.
        - Exit the loop.

    - **Create Token:**

      - **Function Call:**

        ```c
        t_token *create_token(TOKEN_WORD, "-l", gcl);
        ```

      - **Token Created:**

        - `type`: `TOKEN_WORD`
        - `token`: `"-l"`
        - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `"-l"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

  - **Process:**

    - Traverse the token list to find the last token.
    - Append `new_token` to the end of the list.

- **Variables After Second Iteration:**

  - `tokens`: Head of the token list (`"ls" -> "-l"`)
  - `input`: Points to `' '` (space)

---

#### **4. Third Iteration**

- **Skip Whitespace:**

  - Call `skip_whitespace(&input);`
  - Skip the space; `input` now points to `'/'`.

- **Determine Token Type:**

  - Since `is_quote(*input)` is `false` and `is_operator(*input)` is `false`, we proceed to `tokenize_word`.

- **Tokenize Word:**

  - **Function Call:**

    ```c
    t_token *tokenize_word(const char **input, t_gc *gcl);
    ```

  - **Variables:**

    - `start`: Pointer to `'/'`
    - `value`: Empty string buffer.

  - **Process:**

    - **Collect Characters Until Whitespace or Operator:**

      - **Character `'/'`:**

        - Not whitespace, operator, or special.
        - Append `'/'` to `value`: `value = "/"`
        - Advance `*input` to `'h'`.

      - **Character `'h'`:**

        - Not whitespace, operator, or special.
        - Append `'h'` to `value`: `value = "/h"`
        - Advance `*input` to `'o'`.

      - **Character `'o'`:**

        - Append `'o'` to `value`: `value = "/ho"`
        - Advance `*input`.

      - **Characters `'m'`, `'e'`:**

        - Similarly, append `'m'` and `'e'` to `value`: `value = "/home"`
        - Advance `*input` after each character.

      - **End of Input:**

        - `*input` is now `'\0'` (end of string).
        - Exit the loop.

    - **Create Token:**

      - **Function Call:**

        ```c
        t_token *create_token(TOKEN_WORD, "/home", gcl);
        ```

      - **Token Created:**

        - `type`: `TOKEN_WORD`
        - `token`: `"/home"`
        - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `"/home"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

  - **Process:**

    - Traverse the token list to find the last token.
    - Append `new_token` to the end of the list.

- **Variables After Third Iteration:**

  - `tokens`: Head of the token list (`"ls" -> "-l" -> "/home"`)
  - `input`: `'\0'` (end of string)

---

#### **5. End of Tokenization**

- **Check for More Input:**

  - Since `*input` is `'\0'`, we have reached the end of the input.

- **Return Token List:**

  - `tokens`: Pointer to the head of the token list.

---

### **Final Token List**

- **First Token:**

  - `type`: `TOKEN_WORD`
  - `token`: `"ls"`

- **Second Token:**

  - `type`: `TOKEN_WORD`
  - `token`: `"-l"`

- **Third Token:**

  - `type`: `TOKEN_WORD`
  - `token`: `"/home"`

---

## **Example 2: Command with Quotes**

**Command Line Input:**

```
echo "Hello World"
```

**Expected Tokens:**

1. `TOKEN_WORD`: `"echo"`
2. `TOKEN_WORD`: `"Hello World"`

---

### **Detailed Tokenization Steps**

#### **1. Initial Setup**

- **Variables:**

  - `command_line`: `"echo "Hello World""`
  - `input`: Pointer to the first character `'e'`.
  - `tokens`: `NULL`
  - `new_token`: `NULL`

---

#### **2. First Iteration**

- **Current Input Position:**

  - `*input`: `'e'`

- **Process:**

  - **Skip Whitespace:**

    - No leading whitespace; `input` remains at `'e'`.

  - **Determine Token Type:**

    - `is_quote(*input)` is `false`
    - `is_operator(*input)` is `false`
    - Proceed to `tokenize_word`

- **Tokenize Word ("echo"):**

  - **Variables:**

    - `start`: Pointer to `'e'`
    - `value`: Empty string buffer.

  - **Process:**

    - **Collect Characters Until Whitespace or Operator:**

      - **Characters `'e'`, `'c'`, `'h'`, `'o'`:**

        - Append each character to `value`: `value = "echo"`
        - Advance `*input` after each character.

      - **Character `' '`:**

        - Whitespace detected.
        - Exit the loop.

    - **Create Token:**

      - **Function Call:**

        ```c
        t_token *create_token(TOKEN_WORD, "echo", gcl);
        ```

      - **Token Created:**

        - `type`: `TOKEN_WORD`
        - `token`: `"echo"`
        - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `"echo"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

- **Variables After First Iteration:**

  - `tokens`: Head of the token list (`"echo"`)
  - `input`: Points to `' '` (space)

---

#### **3. Second Iteration**

- **Skip Whitespace:**

  - Skip the space; `input` now points to `'"'` (double quote).

- **Determine Token Type:**

  - `is_quote(*input)` is `true` (since `*input` is `'"'`)

- **Tokenize Quote:**

  - **Function Call:**

    ```c
    t_token *tokenize_quote(const char **input, t_gc *gcl);
    ```

  - **Variables:**

    - `quote_char`: `'"'`
    - `value`: Empty string buffer.
    - Advance `*input` to the character after the quote.

  - **Process:**

    - **Collect Characters Until Closing Quote:**

      - **Character `'H'`:**

        - Append `'H'` to `value`: `value = "H"`
        - Advance `*input`.

      - **Characters `'e'`, `'l'`, `'l'`, `'o'`, `' '`, `'W'`, `'o'`, `'r'`, `'l'`, `'d'`:**

        - Append each character to `value`, forming `value = "Hello World"`
        - Advance `*input` after each character.

      - **Character `'"'`:**

        - Closing quote detected.
        - Exit the loop.

    - **Advance `*input`** past the closing quote.

  - **Create Token:**

    - **Function Call:**

      ```c
      t_token *create_token(TOKEN_WORD, "Hello World", gcl);
      ```

    - **Token Created:**

      - `type`: `TOKEN_WORD`
      - `token`: `"Hello World"`
      - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `"Hello World"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

- **Variables After Second Iteration:**

  - `tokens`: Head of the token list (`"echo" -> "Hello World"`)
  - `input`: `'\0'` (end of string)

---

#### **4. End of Tokenization**

- **Check for More Input:**

  - Since `*input` is `'\0'`, we have reached the end of the input.

- **Return Token List:**

  - `tokens`: Pointer to the head of the token list.

---

### **Final Token List**

- **First Token:**

  - `type`: `TOKEN_WORD`
  - `token`: `"echo"`

- **Second Token:**

  - `type`: `TOKEN_WORD`
  - `token`: `"Hello World"`

---

## **Example 3: Command with Redirections**

**Command Line Input:**

```
grep 'pattern' < input.txt > output.txt
```

**Expected Tokens:**

1. `TOKEN_WORD`: `"grep"`
2. `TOKEN_WORD`: `"pattern"`
3. `TOKEN_REDIRECTION`: `"<"`
4. `TOKEN_WORD`: `"input.txt"`
5. `TOKEN_REDIRECTION`: `">"`
6. `TOKEN_WORD`: `"output.txt"`

---

### **Detailed Tokenization Steps**

#### **1. Initial Setup**

- **Variables:**

  - `command_line`: `"grep 'pattern' < input.txt > output.txt"`
  - `input`: Pointer to `'g'`
  - `tokens`: `NULL`
  - `new_token`: `NULL`

---

#### **2. First Iteration (Tokenizing "grep")**

- **Current Input Position:**

  - `*input`: `'g'`

- **Process:**

  - **Skip Whitespace:**

    - No leading whitespace; `input` remains at `'g'`.

  - **Determine Token Type:**

    - `is_quote(*input)` is `false`
    - `is_operator(*input)` is `false`
    - Proceed to `tokenize_word`

- **Tokenize Word ("grep"):**

  - **Variables:**

    - `start`: Pointer to `'g'`
    - `value`: Empty string buffer.

  - **Process:**

    - **Collect Characters Until Whitespace or Operator:**

      - **Characters `'g'`, `'r'`, `'e'`, `'p'`:**

        - Append each character to `value`: `value = "grep"`
        - Advance `*input` after each character.

      - **Character `' '`:**

        - Whitespace detected.
        - Exit the loop.

    - **Create Token:**

      - **Function Call:**

        ```c
        t_token *create_token(TOKEN_WORD, "grep", gcl);
        ```

      - **Token Created:**

        - `type`: `TOKEN_WORD`
        - `token`: `"grep"`
        - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `"grep"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

- **Variables After First Iteration:**

  - `tokens`: `"grep"`
  - `input`: Points to `' '` (space)

---

#### **3. Second Iteration (Tokenizing "'pattern'")**

- **Skip Whitespace:**

  - Skip the space; `input` now points to `'\'` (single quote).

- **Determine Token Type:**

  - `is_quote(*input)` is `true` (since `*input` is `'\'`)

- **Tokenize Quote:**

  - **Function Call:**

    ```c
    t_token *tokenize_quote(const char **input, t_gc *gcl);
    ```

  - **Variables:**

    - `quote_char`: `'\'`
    - `value`: Empty string buffer.
    - Advance `*input` to the character after the quote.

  - **Process:**

    - **Collect Characters Until Closing Quote:**

      - **Characters `'p'`, `'a'`, `'t'`, `'t'`, `'e'`, `'r'`, `'n'`:**

        - Append each character to `value`, forming `value = "pattern"`
        - Advance `*input` after each character.

      - **Character `'\'`:**

        - Closing quote detected.
        - Exit the loop.

    - **Advance `*input`** past the closing quote.

  - **Create Token:**

    - **Function Call:**

      ```c
      t_token *create_token(TOKEN_WORD, "pattern", gcl);
      ```

    - **Token Created:**

      - `type`: `TOKEN_WORD`
      - `token`: `"pattern"`
      - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `"pattern"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

- **Variables After Second Iteration:**

  - `tokens`: `"grep" -> "pattern"`
  - `input`: Points to `' '` (space)

---

#### **4. Third Iteration (Tokenizing `<`)**

- **Skip Whitespace:**

  - Skip the space; `input` now points to `'<'`.

- **Determine Token Type:**

  - `is_operator(*input)` is `true` (since `*input` is `'<'`)

- **Tokenize Operator:**

  - **Function Call:**

    ```c
    t_token *tokenize_operator(const char **input, t_gc *gcl);
    ```

  - **Variables:**

    - `start`: Pointer to `'<'`
    - `value`: Empty string buffer.

  - **Process:**

    - **Collect Operator Characters:**

      - **Character `'<'`:**

        - Operator character.
        - Append `'<'` to `value`: `value = "<"`
        - Advance `*input` to `' '` (space).

      - **No More Operator Characters**.

    - **Map Operator Token Type:**

      - **Function Call:**

        ```c
        t_token_type type = map_token_type_to_operator("<");
        ```

      - **Operator `<` maps to `TOKEN_REDIRECTION`**

    - **Create Token:**

      - **Function Call:**

        ```c
        t_token *create_token(TOKEN_REDIRECTION, "<", gcl);
        ```

      - **Token Created:**

        - `type`: `TOKEN_REDIRECTION`
        - `token`: `"<"`
        - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `"<"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

- **Variables After Third Iteration:**

  - `tokens`: `"grep" -> "pattern" -> "<"`
  - `input`: Points to `' '` (space)

---

#### **5. Fourth Iteration (Tokenizing "input.txt")**

- **Skip Whitespace:**

  - Skip the space; `input` now points to `'i'`.

- **Determine Token Type:**

  - Proceed to `tokenize_word` since `is_quote` and `is_operator` are `false`.

- **Tokenize Word ("input.txt"):**

  - **Variables:**

    - `start`: Pointer to `'i'`
    - `value`: Empty string buffer.

  - **Process:**

    - **Collect Characters Until Whitespace or Operator:**

      - **Characters `'i'`, `'n'`, `'p'`, `'u'`, `'t'`, `'.'`, `'t'`, `'x'`, `'t'`:**

        - Append each character to `value`, forming `value = "input.txt"`
        - Advance `*input` after each character.

      - **Character `' '` (space):**

        - Whitespace detected.
        - Exit the loop.

    - **Create Token:**

      - **Function Call:**

        ```c
        t_token *create_token(TOKEN_WORD, "input.txt", gcl);
        ```

      - **Token Created:**

        - `type`: `TOKEN_WORD`
        - `token`: `"input.txt"`
        - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `"input.txt"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

- **Variables After Fourth Iteration:**

  - `tokens`: `"grep" -> "pattern" -> "<" -> "input.txt"`
  - `input`: Points to `' '` (space)

---

#### **6. Fifth Iteration (Tokenizing `>`)**

- **Skip Whitespace:**

  - Skip the space; `input` now points to `'>'`.

- **Determine Token Type:**

  - `is_operator(*input)` is `true`

- **Tokenize Operator (`>`):**

  - **Variables:**

    - `start`: Pointer to `'>'`
    - `value`: Empty string buffer.

  - **Process:**

    - **Collect Operator Characters:**

      - **Character `'>'`:**

        - Operator character.
        - Append `'>'` to `value`: `value = ">"`
        - Advance `*input` to `' '` (space).

    - **Map Operator Token Type:**

      - **Operator `>` maps to `TOKEN_REDIRECTION`**

    - **Create Token:**

      - **Function Call:**

        ```c
        t_token *create_token(TOKEN_REDIRECTION, ">", gcl);
        ```

      - **Token Created:**

        - `type`: `TOKEN_REDIRECTION`
        - `token`: `">"`
        - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `">"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

- **Variables After Fifth Iteration:**

  - `tokens`: `"grep" -> "pattern" -> "<" -> "input.txt" -> ">"`
  - `input`: Points to `' '` (space)

---

#### **7. Sixth Iteration (Tokenizing "output.txt")**

- **Skip Whitespace:**

  - Skip the space; `input` now points to `'o'`.

- **Determine Token Type:**

  - Proceed to `tokenize_word`.

- **Tokenize Word ("output.txt"):**

  - **Variables:**

    - `start`: Pointer to `'o'`
    - `value`: Empty string buffer.

  - **Process:**

    - **Collect Characters Until Whitespace or Operator:**

      - **Characters `'o'`, `'u'`, `'t'`, `'p'`, `'u'`, `'t'`, `'.'`, `'t'`, `'x'`, `'t'`:**

        - Append each character to `value`, forming `value = "output.txt"`
        - Advance `*input` after each character.

      - **End of Input:**

        - `*input` is now `'\0'`
        - Exit the loop.

    - **Create Token:**

      - **Function Call:**

        ```c
        t_token *create_token(TOKEN_WORD, "output.txt", gcl);
        ```

      - **Token Created:**

        - `type`: `TOKEN_WORD`
        - `token`: `"output.txt"`
        - `next`: `NULL`

  - **Return Token:**

    - `new_token`: Pointer to the token created for `"output.txt"`

- **Add Token to List:**

  - **Function Call:**

    ```c
    add_token(&tokens, new_token);
    ```

- **Variables After Sixth Iteration:**

  - `tokens`: `"grep" -> "pattern" -> "<" -> "input.txt" -> ">" -> "output.txt"`
  - `input`: `'\0'` (end of input)

---

#### **8. End of Tokenization**

- **Check for More Input:**

  - `*input` is `'\0'`

- **Return Token List:**

  - `tokens`: Pointer to the head of the token list.

---

### **Final Token List**

- **Tokens:**

  1. `TOKEN_WORD`: `"grep"`
  2. `TOKEN_WORD`: `"pattern"`
  3. `TOKEN_REDIRECTION`: `"<"`
  4. `TOKEN_WORD`: `"input.txt"`
  5. `TOKEN_REDIRECTION`: `">"`
  6. `TOKEN_WORD`: `"output.txt"`

---

## **Understanding Variable Values and Data Flow**

Throughout the tokenization process, we kept track of variables like `*input`, `tokens`, `new_token`, and `value` (the string being built). By advancing `*input` and appending characters to `value` as appropriate, the tokenizer constructs tokens that represent the meaningful elements of the command line.

---

## **Summary**

- The tokenizer reads the input string character by character.
- It uses utility functions to determine the type of each character.
- Depending on the character, it calls the appropriate tokenization function:
  - `tokenize_word` for words.
  - `tokenize_quote` for quoted strings.
  - `tokenize_operator` for operators.
- It collects characters until it encounters a delimiter (whitespace, operator, etc.).
- It creates tokens and adds them to the token list.
- This process continues until the end of the input string.

 * */
