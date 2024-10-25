/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_parse_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:31:13 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/25 17:06:01 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/ast.h"
#include "../../lib/libgc/include/libgc.h"
#include "../../lib/libft/include/libft.h"
#include <sys/types.h>
#include <sys/wait.h>

/* Global garbage collector */
t_gc *gcl = NULL;

/* Helper function to create a linked list of tokens using gc_malloc */
t_token *create_tokens(int count, ...)
{
    va_list args;
    t_token *head = NULL;
    t_token *current = NULL;
    int i;

    va_start(args, count);
    for (i = 0; i < count; i++)
    {
        int type = va_arg(args, int);
        char *token_str = va_arg(args, char *);

        t_token *new_token = gc_malloc(sizeof(t_token), gcl);
        new_token->type = type;
        new_token->token = token_str;
        new_token->next = NULL;

        if (!head)
        {
            head = new_token;
            current = head;
        }
        else
        {
            current->next = new_token;
            current = current->next;
        }
    }
    va_end(args);
    return head;
}

START_TEST(test_create_command_node)
{
    /* Mock tokens representing the command "echo hello" */
    t_token *tokens = create_tokens(2,
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello");
    t_token *current_token = tokens;

    t_ast_node *cmd_node = create_command_node(&current_token, gcl);

    ck_assert_ptr_nonnull(cmd_node);
    ck_assert_int_eq(cmd_node->type, NODE_COMMAND);
    ck_assert_int_eq(cmd_node->data.command.argc, 2);
    ck_assert_str_eq(cmd_node->data.command.argv[0], "echo");
    ck_assert_str_eq(cmd_node->data.command.argv[1], "hello");
    ck_assert_ptr_null(cmd_node->data.command.argv[2]);
}
END_TEST

START_TEST(test_parse_simple_command)
{
    /* Mock tokens representing the command "ls" */
    t_token *tokens = create_tokens(1, TOKEN_WORD, "ls");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_command(&current_token, gcl);

    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_COMMAND);
    ck_assert_int_eq(ast->data.command.argc, 1);
    ck_assert_str_eq(ast->data.command.argv[0], "ls");
}
END_TEST

START_TEST(test_parse_pipeline)
{
    /* Mock tokens representing the command "ls | grep src" */
    t_token *tokens = create_tokens(4,
                                    TOKEN_WORD, "ls",
                                    TOKEN_PIPE, "|",
                                    TOKEN_WORD, "grep",
                                    TOKEN_WORD, "src");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_pipeline(&current_token, gcl);

    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_PIPE);

    /* Check left command "ls" */
    ck_assert_int_eq(ast->data.pipe.left->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.pipe.left->data.command.argv[0], "ls");

    /* Check right command "grep src" */
    ck_assert_int_eq(ast->data.pipe.right->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.pipe.right->data.command.argv[0], "grep");
    ck_assert_str_eq(ast->data.pipe.right->data.command.argv[1], "src");
}
END_TEST

START_TEST(test_parse_redirection)
{
    /* Mock tokens representing the command "echo hello > output.txt" */
    t_token *tokens = create_tokens(4,
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello",
                                    TOKEN_REDIRECTION, ">",
                                    TOKEN_WORD, "output.txt");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_command(&current_token, gcl);
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_REDIRECT_OUT);

    /* Check child command "echo hello" */
    ck_assert_int_eq(ast->data.redirect.child->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.redirect.child->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.redirect.child->data.command.argv[1], "hello");

    /* Check redirection filename */
    ck_assert_str_eq(ast->data.redirect.filename, "output.txt");
}
END_TEST

START_TEST(test_parse_logical)
{
    /* Mock tokens representing the command "echo hello && echo world" */
    t_token *tokens = create_tokens(5,
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello",
                                    TOKEN_AND, "&&",
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "world");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_logical(&current_token, gcl);
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_AND);

    /* Check left command "echo hello" */
    ck_assert_int_eq(ast->data.logic.left->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.logic.left->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.logic.left->data.command.argv[1], "hello");

    /* Check right command "echo world" */
    ck_assert_int_eq(ast->data.logic.right->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.logic.right->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.logic.right->data.command.argv[1], "world");
}
END_TEST

START_TEST(test_parse_subshell)
{
    /* Mock tokens representing the command "(echo hello)" */
    t_token *tokens = create_tokens(4,
                                    TOKEN_SUBSHELL_START, "(",
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello",
                                    TOKEN_SUBSHELL_STOP, ")");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_subshell(&current_token, gcl);

    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_SUBSHELL);
    ck_assert_int_eq(ast->data.subshell.child->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.subshell.child->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.subshell.child->data.command.argv[1], "hello");
}
END_TEST

START_TEST(test_parse_heredoc)
{
    /* Mock tokens representing the command "cat << EOF" */
    t_token *tokens = create_tokens(3,
                                    TOKEN_WORD, "cat",
                                    TOKEN_REDIRECTION, "<<",
                                    TOKEN_WORD, "EOF");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_command(&current_token, gcl);
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_REDIRECT_HEREDOC);

    /* Check child command "cat" */
    ck_assert_int_eq(ast->data.heredoc.child->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.heredoc.child->data.command.argv[0], "cat");

    /* Check heredoc delimiter */
    ck_assert_str_eq(ast->data.heredoc.delimiter, "EOF");
}
END_TEST

START_TEST(test_parse_complex_command)
{
    /* Mock tokens representing the command "echo hello | grep h && (ls -l > out.txt)" */
    t_token *tokens = create_tokens(15,
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello",
                                    TOKEN_PIPE, "|",
                                    TOKEN_WORD, "grep",
                                    TOKEN_WORD, "h",
                                    TOKEN_AND, "&&",
                                    TOKEN_SUBSHELL_START, "(",
                                    TOKEN_WORD, "ls",
                                    TOKEN_WORD, "-l",
                                    TOKEN_REDIRECTION, ">",
                                    TOKEN_WORD, "out.txt",
                                    TOKEN_SUBSHELL_STOP, ")");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_logical(&current_token, gcl);
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_AND);

    /* Check left side "echo hello | grep h" */
    t_ast_node *left = ast->data.logic.left;
    ck_assert_int_eq(left->type, NODE_PIPE);
    ck_assert_str_eq(left->data.pipe.left->data.command.argv[0], "echo");
    ck_assert_str_eq(left->data.pipe.left->data.command.argv[1], "hello");
    ck_assert_str_eq(left->data.pipe.right->data.command.argv[0], "grep");
    ck_assert_str_eq(left->data.pipe.right->data.command.argv[1], "h");

    /* Check right side "(ls -l > out.txt)" */
    t_ast_node *right = ast->data.logic.right;
    ck_assert_int_eq(right->type, NODE_SUBSHELL);
    t_ast_node *subshell_child = right->data.subshell.child;
    ck_assert_int_eq(subshell_child->type, NODE_REDIRECT_OUT);
    ck_assert_str_eq(subshell_child->data.redirect.filename, "out.txt");
    ck_assert_str_eq(subshell_child->data.redirect.child->data.command.argv[0], "ls");
    ck_assert_str_eq(subshell_child->data.redirect.child->data.command.argv[1], "-l");
}
END_TEST

START_TEST(test_parse_syntax_error)
{
    /* Mock tokens representing an incomplete command "echo hello |" */
    t_token *tokens = create_tokens(3,
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello",
                                    TOKEN_PIPE, "|");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_pipeline(&current_token, gcl);
    ck_assert_ptr_null(ast);
}
END_TEST

START_TEST(test_parse_unexpected_token)
{
    /* Mock tokens representing an unexpected token sequence "&& echo hello" */
    t_token *tokens = create_tokens(3,
                                    TOKEN_AND, "&&",
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_logical(&current_token, gcl);
    ck_assert_ptr_null(ast);
}
END_TEST

START_TEST(test_parse_empty_input)
{
    pid_t pid = fork();
    ck_assert_int_ne(pid, -1); // Ensure fork was successful

    if (pid == 0)
    {
        // Child process
        t_token *current_token = NULL;
        ms_parse_tokens(current_token, gcl);
        // If ms_parse_tokens doesn't exit, exit with success
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            // Expecting the child to exit with EXIT_FAILURE
            ck_assert_int_eq(exit_status, EXIT_FAILURE);
        }
        else if (WIFSIGNALED(status))
        {
            int term_sig = WTERMSIG(status);
            // If the child was terminated by SIGSEGV, the test fails
            ck_assert_msg(term_sig != SIGSEGV, "Segmentation fault occurred when parsing empty input.");
            // If terminated by another signal, report it
            ck_assert_msg(0, "Child process terminated by signal %d", term_sig);
        }
        else
        {
            // Child process did not exit normally
            ck_assert_msg(0, "Child process did not exit normally.");
        }
    }
}
END_TEST

Suite *ast_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("AST");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Add tests to the test case */
    tcase_add_test(tc_core, test_create_command_node);
    tcase_add_test(tc_core, test_parse_simple_command);
    tcase_add_test(tc_core, test_parse_pipeline);
    tcase_add_test(tc_core, test_parse_redirection);
    tcase_add_test(tc_core, test_parse_logical);
    tcase_add_test(tc_core, test_parse_subshell);
    tcase_add_test(tc_core, test_parse_heredoc);
    tcase_add_test(tc_core, test_parse_complex_command);
    tcase_add_test(tc_core, test_parse_syntax_error);
    tcase_add_test(tc_core, test_parse_unexpected_token);
    tcase_add_test(tc_core, test_parse_empty_input);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    /* Initialize the garbage collector */
    gcl = gc_init();

    s = ast_suite();
    sr = srunner_create(s);

    /* Run tests with normal verbosity */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    /* Clean up the garbage collector */
    gc_cleanup(gcl);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
