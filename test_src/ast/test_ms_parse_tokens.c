/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_parse_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:31:13 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/24 08:31:27 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/ast.h"
#include "../../lib/libgc/include/libgc.h"

// Mock data functions for building AST nodes
static t_ast_node *mock_command_node(const char *command, t_gc *gcl)
{
    t_token tokens[1] = {
        {TOKEN_WORD, command, NULL}
    };
    t_token *current_token = tokens;
    return create_command_node(&current_token, gcl);
}

static t_ast_node *mock_pipe_node(const char *left_command, const char *right_command, t_gc *gcl)
{
    t_ast_node *left = mock_command_node(left_command, gcl);
    t_ast_node *right = mock_command_node(right_command, gcl);
    return create_pipe_node(left, right, gcl);
}

static t_ast_node *mock_redirect_node(const char *command, const char *filename, t_node_type type, t_gc *gcl)
{
    t_ast_node *child = mock_command_node(command, gcl);
    return create_redirect_node(type, child, filename, gcl);
}

static t_ast_node *mock_subshell_node(const char *command, t_gc *gcl)
{
    t_ast_node *child = mock_command_node(command, gcl);
    return create_subshell_node(child, gcl);
}

static t_ast_node *mock_background_node(const char *command, t_gc *gcl)
{
    t_ast_node *child = mock_command_node(command, gcl);
    return create_background_node(child, gcl);
}

START_TEST(test_create_command_node)
{
    t_gc *gcl = gc_init();
    t_token tokens[2] = {
        {TOKEN_WORD, "echo", NULL},
        {TOKEN_WORD, "Hello", NULL}
    };
    t_token *current_token = tokens;

    t_ast_node *node = create_command_node(&current_token, gcl);

    ck_assert_ptr_nonnull(node);
    ck_assert_int_eq(node->type, NODE_COMMAND);
    ck_assert_str_eq(node->data.command.argv[0], "echo");
    ck_assert_str_eq(node->data.command.argv[1], "Hello");
    ck_assert_int_eq(node->data.command.argc, 2);

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_create_pipe_node)
{
    t_gc *gcl = gc_init();
    t_ast_node *pipe_node = mock_pipe_node("echo", "grep", gcl);

    ck_assert_ptr_nonnull(pipe_node);
    ck_assert_int_eq(pipe_node->type, NODE_PIPE);
    ck_assert_ptr_nonnull(pipe_node->data.pipe.left);
    ck_assert_ptr_nonnull(pipe_node->data.pipe.right);
    ck_assert_str_eq(pipe_node->data.pipe.left->data.command.argv[0], "echo");
    ck_assert_str_eq(pipe_node->data.pipe.right->data.command.argv[0], "grep");

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_create_redirect_node)
{
    t_gc *gcl = gc_init();
    t_ast_node *redirect_node = mock_redirect_node("echo", "output.txt", NODE_REDIRECT_OUT, gcl);

    ck_assert_ptr_nonnull(redirect_node);
    ck_assert_int_eq(redirect_node->type, NODE_REDIRECT_OUT);
    ck_assert_ptr_nonnull(redirect_node->data.redirect.child);
    ck_assert_str_eq(redirect_node->data.redirect.filename, "output.txt");
    ck_assert_str_eq(redirect_node->data.redirect.child->data.command.argv[0], "echo");

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_create_subshell_node)
{
    t_gc *gcl = gc_init();
    t_ast_node *subshell_node = mock_subshell_node("ls -l", gcl);

    ck_assert_ptr_nonnull(subshell_node);
    ck_assert_int_eq(subshell_node->type, NODE_SUBSHELL);
    ck_assert_ptr_nonnull(subshell_node->data.subshell.child);
    ck_assert_str_eq(subshell_node->data.subshell.child->data.command.argv[0], "ls");
    ck_assert_str_eq(subshell_node->data.subshell.child->data.command.argv[1], "-l");

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_create_background_node)
{
    t_gc *gcl = gc_init();
    t_ast_node *background_node = mock_background_node("sleep 10", gcl);

    ck_assert_ptr_nonnull(background_node);
    ck_assert_int_eq(background_node->type, NODE_BACKGROUND);
    ck_assert_ptr_nonnull(background_node->data.background.child);
    ck_assert_str_eq(background_node->data.background.child->data.command.argv[0], "sleep");
    ck_assert_str_eq(background_node->data.background.child->data.command.argv[1], "10");

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_parse_simple_command)
{
    t_gc *gcl = gc_init();
    t_token tokens[2] = {
        {TOKEN_WORD, "echo", NULL},
        {TOKEN_WORD, "Hello", NULL}
    };
    t_token *current_token = tokens;

    t_ast_node *node = parse_command(&current_token, gcl);

    ck_assert_ptr_nonnull(node);
    ck_assert_int_eq(node->type, NODE_COMMAND);
    ck_assert_str_eq(node->data.command.argv[0], "echo");
    ck_assert_str_eq(node->data.command.argv[1], "Hello");
    ck_assert_int_eq(node->data.command.argc, 2);

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_parse_pipeline)
{
    t_gc *gcl = gc_init();
    t_token tokens[4] = {
        {TOKEN_WORD, "echo", NULL},
        {TOKEN_WORD, "Hello", NULL},
        {TOKEN_PIPE, "|", NULL},
        {TOKEN_WORD, "grep", NULL}
    };
    t_token *current_token = tokens;

    t_ast_node *pipeline_node = parse_pipeline(&current_token, gcl);

    ck_assert_ptr_nonnull(pipeline_node);
    ck_assert_int_eq(pipeline_node->type, NODE_PIPE);
    ck_assert_ptr_nonnull(pipeline_node->data.pipe.left);
    ck_assert_ptr_nonnull(pipeline_node->data.pipe.right);

    ck_assert_int_eq(pipeline_node->data.pipe.left->type, NODE_COMMAND);
    ck_assert_str_eq(pipeline_node->data.pipe.left->data.command.argv[0], "echo");
    ck_assert_str_eq(pipeline_node->data.pipe.left->data.command.argv[1], "Hello");

    ck_assert_int_eq(pipeline_node->data.pipe.right->type, NODE_COMMAND);
    ck_assert_str_eq(pipeline_node->data.pipe.right->data.command.argv[0], "grep");

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_parse_logical)
{
    t_gc *gcl = gc_init();
    t_token tokens[6] = {
        {TOKEN_WORD, "echo", NULL},
        {TOKEN_WORD, "Hello", NULL},
        {TOKEN_AND, "&&", NULL},
        {TOKEN_WORD, "ls", NULL},
        {TOKEN_OR, "||", NULL},
        {TOKEN_WORD, "pwd", NULL}
    };
    t_token *current_token = tokens;

    t_ast_node *logic_node = parse_logical(&current_token, gcl);

    ck_assert_ptr_nonnull(logic_node);
    ck_assert_int_eq(logic_node->type, NODE_AND);
    ck_assert_ptr_nonnull(logic_node->data.logic.left);
    ck_assert_ptr_nonnull(logic_node->data.logic.right);

    // Validate left side of AND (echo "Hello")
    ck_assert_int_eq(logic_node->data.logic.left->type, NODE_COMMAND);
    ck_assert_str_eq(logic_node->data.logic.left->data.command.argv[0], "echo");
    ck_assert_str_eq(logic_node->data.logic.left->data.command.argv[1], "Hello");

    // Validate right side (ls || pwd)
    t_ast_node *right = logic_node->data.logic.right;
    ck_assert_int_eq(right->type, NODE_OR);
    ck_assert_ptr_nonnull(right->data.logic.left);
    ck_assert_ptr_nonnull(right->data.logic.right);

    // Validate left of OR (ls)
    ck_assert_int_eq(right->data.logic.left->type, NODE_COMMAND);
    ck_assert_str_eq(right->data.logic.left->data.command.argv[0], "ls");

    // Validate right of OR (pwd)
    ck_assert_int_eq(right->data.logic.right->type, NODE_COMMAND);
    ck_assert_str_eq(right->data.logic.right->data.command.argv[0], "pwd");

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_ms_parse_tokens_simple)
{
    t_gc *gcl = gc_init();
    t_token tokens[4] = {
        {TOKEN_WORD, "echo", NULL},
        {TOKEN_WORD, "Hello", NULL},
        {TOKEN_AND, "&&", NULL},
        {TOKEN_WORD, "ls", NULL}
    };

    t_ast_node *ast = ms_parse_tokens(tokens, gcl);

    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_AND);
    ck_assert_ptr_nonnull(ast->data.logic.left);
    ck_assert_ptr_nonnull(ast->data.logic.right);

    ck_assert_int_eq(ast->data.logic.left->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.logic.left->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.logic.left->data.command.argv[1], "Hello");

    ck_assert_int_eq(ast->data.logic.right->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.logic.right->data.command.argv[0], "ls");

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_ms_parse_tokens_complex)
{
    t_gc *gcl = gc_init();
    t_token tokens[9] = {
        {TOKEN_WORD, "echo", NULL},
        {TOKEN_WORD, "Hello", NULL},
        {TOKEN_AND, "&&", NULL},
        {TOKEN_WORD, "ls", NULL},
        {TOKEN_PIPE, "|", NULL},
        {TOKEN_WORD, "grep", NULL},
        {TOKEN_WORD, "file", NULL},
        {TOKEN_REDIRECTION, ">", NULL},
        {TOKEN_WORD, "output.txt", NULL}
    };

    t_ast_node *ast = ms_parse_tokens(tokens, gcl);

    ck_assert_ptr_nonnull(ast);
    ck_assert_int_eq(ast->type, NODE_AND);
    ck_assert_ptr_nonnull(ast->data.logic.left);
    ck_assert_ptr_nonnull(ast->data.logic.right);

    // Validate left side of AND (echo "Hello")
    ck_assert_int_eq(ast->data.logic.left->type, NODE_COMMAND);
    ck_assert_str_eq(ast->data.logic.left->data.command.argv[0], "echo");
    ck_assert_str_eq(ast->data.logic.left->data.command.argv[1], "Hello");

    // Validate right side of AND (ls | grep "file" > output.txt)
    t_ast_node *right = ast->data.logic.right;
    ck_assert_int_eq(right->type, NODE_PIPE);
    ck_assert_ptr_nonnull(right->data.pipe.left);
    ck_assert_ptr_nonnull(right->data.pipe.right);

    // Validate pipe left (ls)
    ck_assert_int_eq(right->data.pipe.left->type, NODE_COMMAND);
    ck_assert_str_eq(right->data.pipe.left->data.command.argv[0], "ls");

    // Validate pipe right (grep "file" > output.txt)
    t_ast_node *pipe_right = right->data.pipe.right;
    ck_assert_int_eq(pipe_right->type, NODE_REDIRECT_OUT);
    ck_assert_ptr_nonnull(pipe_right->data.redirect.child);
    ck_assert_str_eq(pipe_right->data.redirect.filename, "output.txt");

    // Validate redirect child (grep "file")
    ck_assert_int_eq(pipe_right->data.redirect.child->type, NODE_COMMAND);
    ck_assert_str_eq(pipe_right->data.redirect.child->data.command.argv[0], "grep");
    ck_assert_str_eq(pipe_right->data.redirect.child->data.command.argv[1], "file");

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_parse_empty_input)
{
    t_gc *gcl = gc_init();
    t_token tokens[1] = {{TOKEN_END, NULL, NULL}};
    t_ast_node *ast = ms_parse_tokens(tokens, gcl);

    ck_assert_ptr_null(ast);

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_parse_incomplete_pipeline)
{
    t_gc *gcl = gc_init();
    t_token tokens[3] = {
        {TOKEN_WORD, "echo", NULL},
        {TOKEN_WORD, "Hello", NULL},
        {TOKEN_PIPE, "|", NULL}
    };
    t_ast_node *ast = ms_parse_tokens(tokens, gcl);

    ck_assert_ptr_null(ast);

    gc_cleanup(gcl);
}
END_TEST

Suite *ast_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("AST");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_create_command_node);
    tcase_add_test(tc_core, test_create_pipe_node);
    tcase_add_test(tc_core, test_create_redirect_node);
    tcase_add_test(tc_core, test_create_subshell_node);
    tcase_add_test(tc_core, test_create_background_node);
    tcase_add_test(tc_core, test_parse_simple_command);
    tcase_add_test(tc_core, test_parse_pipeline);
    tcase_add_test(tc_core, test_parse_logical);
    tcase_add_test(tc_core, test_ms_parse_tokens_simple);
    tcase_add_test(tc_core, test_ms_parse_tokens_complex);
    tcase_add_test(tc_core, test_parse_empty_input);
    tcase_add_test(tc_core, test_parse_incomplete_pipeline);
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
