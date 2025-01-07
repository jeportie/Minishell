/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_parse_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:31:13 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/06 10:41:32 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/ast.h"
#include "../../lib/libgc/include/libgc.h"
#include "../../lib/libft/include/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

static t_gc *gcl;

static t_shell shell;

void setup(void)
{
    gcl = gc_init();
    if (!gcl)
    {
        perror("gc_init");
        exit(EXIT_FAILURE);
    }
    shell.env_data = NULL;
    shell.user_input = NULL;
	shell.error_code = 1;
	shell.interactive_mode = 0;
    shell.gcl = gcl;
}

void teardown(void)
{
    gc_cleanup(gcl);
}

t_token *create_tokens(t_gc *gcl, int count, ...)
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
        if (!new_token)
        {
            perror("gc_malloc");
            exit(EXIT_FAILURE);
        }
        new_token->type = type;
        new_token->token = ft_strdup(token_str);
        gc_register(new_token->token, gcl);
        if (!new_token->token)
        {
            perror("gc_strdup");
            exit(EXIT_FAILURE);
        }
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
    t_token *tokens = create_tokens(gcl, 2,
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
    t_token *tokens = create_tokens(gcl, 1, TOKEN_WORD, "ls");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_command(&current_token, &shell, gcl);
    
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_COMMAND);
    ck_assert_int_eq(ast->data.command.argc, 1);
    ck_assert_str_eq(ast->data.command.argv[0], "ls");
    ck_assert_ptr_null(ast->data.command.argv[1]);
}
END_TEST

START_TEST(test_parse_pipeline)
{
    t_token *tokens = create_tokens(gcl, 4,
                                    TOKEN_WORD, "ls",
                                    TOKEN_PIPE, "|",
                                    TOKEN_WORD, "grep",
                                    TOKEN_WORD, "src");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_pipeline(&current_token, &shell, gcl);
    
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_PIPE);

    ck_assert_int_eq(ast->data.pipe.left->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.pipe.left->data.command.argv[0], "ls");
    ck_assert_ptr_null(ast->data.pipe.left->data.command.argv[1]);

    ck_assert_int_eq(ast->data.pipe.right->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.pipe.right->data.command.argv[0], "grep");
    ck_assert_str_eq(ast->data.pipe.right->data.command.argv[1], "src");
    ck_assert_ptr_null(ast->data.pipe.right->data.command.argv[2]);
}
END_TEST

START_TEST(test_parse_redirection)
{
    t_token *tokens = create_tokens(gcl, 4,
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello",
                                    TOKEN_REDIRECTION, ">",
                                    TOKEN_WORD, "output.txt");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_command(&current_token, &shell, gcl);
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_REDIRECT_OUT);

    ck_assert_int_eq(ast->data.redirect.child->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.redirect.child->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.redirect.child->data.command.argv[1], "hello");
    ck_assert_ptr_null(ast->data.redirect.child->data.command.argv[2]);

    ck_assert_str_eq(ast->data.redirect.filename, "output.txt");
}
END_TEST

START_TEST(test_parse_logical)
{
    t_token *tokens = create_tokens(gcl, 5,
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello",
                                    TOKEN_AND, "&&",
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "world");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_logical(&current_token, &shell, gcl);
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_AND);

    ck_assert_int_eq(ast->data.logic.left->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.logic.left->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.logic.left->data.command.argv[1], "hello");
    ck_assert_ptr_null(ast->data.logic.left->data.command.argv[2]);

    ck_assert_int_eq(ast->data.logic.right->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.logic.right->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.logic.right->data.command.argv[1], "world");
    ck_assert_ptr_null(ast->data.logic.right->data.command.argv[2]);
}
END_TEST

START_TEST(test_parse_subshell)
{
    t_token *tokens = create_tokens(gcl, 4,
                                    TOKEN_SUBSHELL_START, "(",
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello",
                                    TOKEN_SUBSHELL_STOP, ")");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_subshell(&current_token, &shell, gcl);
    
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_SUBSHELL);
    ck_assert_int_eq(ast->data.subshell.child->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.subshell.child->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.subshell.child->data.command.argv[1], "hello");
    ck_assert_ptr_null(ast->data.subshell.child->data.command.argv[2]);
}
END_TEST

START_TEST(test_parse_heredoc)
{
    t_token *tokens = create_tokens(gcl, 3,
                                    TOKEN_WORD, "cat",
                                    TOKEN_REDIRECTION, "<<",
                                    TOKEN_WORD, "EOF");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_command(&current_token, &shell, gcl);
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_REDIRECT_HEREDOC);

    ck_assert_int_eq(ast->data.heredoc.child->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.heredoc.child->data.command.argv[0], "cat");
    ck_assert_ptr_null(ast->data.heredoc.child->data.command.argv[1]);

    ck_assert_str_eq(ast->data.heredoc.delimiter, "EOF");
}
END_TEST

START_TEST(test_parse_complex_command)
{
    t_token *tokens = create_tokens(gcl, 12,
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

    t_ast_node *ast = parse_logical(&current_token, &shell, gcl);
    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_AND);

    t_ast_node *left = ast->data.logic.left;
    ck_assert_int_eq(left->type, NODE_PIPE);
    ck_assert_str_eq(left->data.pipe.left->data.command.argv[0], "echo");
    ck_assert_str_eq(left->data.pipe.left->data.command.argv[1], "hello");
    ck_assert_ptr_null(left->data.pipe.left->data.command.argv[2]);

    ck_assert_str_eq(left->data.pipe.right->data.command.argv[0], "grep");
    ck_assert_str_eq(left->data.pipe.right->data.command.argv[1], "h");
    ck_assert_ptr_null(left->data.pipe.right->data.command.argv[2]);

    t_ast_node *right = ast->data.logic.right;
    ck_assert_int_eq(right->type, NODE_SUBSHELL);
    t_ast_node *subshell_child = right->data.subshell.child;
    ck_assert_int_eq(subshell_child->type, NODE_REDIRECT_OUT);
    ck_assert_str_eq(subshell_child->data.redirect.filename, "out.txt");

    ck_assert_str_eq(subshell_child->data.redirect.child->data.command.argv[0], "ls");
    ck_assert_str_eq(subshell_child->data.redirect.child->data.command.argv[1], "-l");
    ck_assert_ptr_null(subshell_child->data.redirect.child->data.command.argv[2]);
}
END_TEST

START_TEST(test_parse_syntax_error)
{
    t_token *tokens = create_tokens(gcl, 3,
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello",
                                    TOKEN_PIPE, "|");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_pipeline(&current_token, &shell, gcl);
    ck_assert_ptr_null(ast);
}
END_TEST

START_TEST(test_parse_unexpected_token)
{
    t_token *tokens = create_tokens(gcl, 3,
                                    TOKEN_AND, "&&",
                                    TOKEN_WORD, "echo",
                                    TOKEN_WORD, "hello");
    t_token *current_token = tokens;

    t_ast_node *ast = parse_logical(&current_token, &shell, gcl);
    ck_assert_ptr_null(ast);
}
END_TEST

START_TEST(test_parse_empty_input)
{
    pid_t pid = fork();
    ck_assert_int_ne(pid, -1);

    if (pid == 0)
    {
        t_gc *child_gcl = gc_init();
        if (!child_gcl)
        {
            perror("gc_init in child");
            exit(EXIT_FAILURE);
        }

        t_token *current_token = NULL;
        t_ast_node *ast = ms_parse_tokens(current_token, &shell, child_gcl);
        if (ast != NULL)
        {
            gc_cleanup(child_gcl);
            exit(EXIT_FAILURE);
        }
        gc_cleanup(child_gcl);
        exit(EXIT_SUCCESS);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            ck_assert_int_eq(exit_status, EXIT_SUCCESS);
        }
        else if (WIFSIGNALED(status))
        {
            int term_sig = WTERMSIG(status);
            ck_assert_msg(term_sig != SIGSEGV, "Segmentation fault occurred when parsing empty input.");
            ck_assert_msg(0, "Child process terminated by signal %d", term_sig);
        }
        else
            ck_assert_msg(0, "Child process did not exit normally.");
    }
}
END_TEST

Suite *ast_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("AST");

    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);

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

    s = ast_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
