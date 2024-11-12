/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_execute_logical.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: assistant <assistant@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 12:00:00 by assistant         #+#    #+#             */
/*   Updated: 2024/11/12 13:22:38 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/exec.h"
#include "../../include/process.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

static t_gc *gcl;

/* Mock function for ms_execute_ast */
int mock_execute_ast(t_ast_node *node, t_exec_context *context, t_proc_manager *manager)
{
    /* For testing, we can simulate command execution based on node type */
    if (node->type == 0) /* Command node */
    {
        t_cmd_node *cmd_node = &node->data.command;

        /* Simulate command execution based on command name */
        if (strcmp(cmd_node->argv[0], "true") == 0)
        {
            return 0; /* Success */
        }
        else if (strcmp(cmd_node->argv[0], "false") == 0)
        {
            return 1; /* Failure */
        }
        else if (strcmp(cmd_node->argv[0], "error") == 0)
        {
            return 2; /* Arbitrary non-zero exit status */
        }
        else
        {
            return 127; /* Command not found */
        }
    }
    else if (node->type == NODE_AND || node->type == NODE_OR)
    {
        /* Logic node */
        return ms_execute_logical(&node->data.logic, context, node->type, manager);
    }
    else
    {
        /* Other node types */
        return -1;
    }
}

/* Helper function to create a command node */
t_ast_node *create_cmd_node(char *command[], t_gc *gcl)
{
    t_ast_node *node = gc_malloc(sizeof(t_ast_node), gcl);
    node->type = 0; /* Command node type */
    node->data.command.argv = command;
    return node;
}

/* Helper function to create a logic node */
t_ast_node *create_logic_node(t_ast_node *left, t_ast_node *right, t_node_type type, t_gc *gcl)
{
    t_ast_node *node = gc_malloc(sizeof(t_ast_node), gcl);
    node->type = type;
    node->data.logic.left = left;
    node->data.logic.right = right;
    return node;
}

/* Setup and Teardown functions */
void setup(void)
{
    gcl = gc_init();
    if (!gcl)
    {
        perror("gc_init");
        exit(EXIT_FAILURE);
    }
}

void teardown(void)
{
    gc_cleanup(gcl);
}

/* Test Cases */

/* Test 1: Logical AND with left command succeeds */
START_TEST(test_ms_execute_logical_and_left_succeeds)
{
    /* Commands: true && true */
    char *true_args[] = {"true", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(true_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(true_args, gcl);

    /* Create logic node */
    t_ast_node *logic_node = create_logic_node(left_cmd, right_cmd, NODE_AND, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_logical(&logic_node->data.logic, &context, NODE_AND, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_eq(context.exit_status, 0);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 2: Logical AND with left command fails */
START_TEST(test_ms_execute_logical_and_left_fails)
{
    /* Commands: false && true */
    char *false_args[] = {"false", NULL};
    char *true_args[] = {"true", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(false_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(true_args, gcl);

    /* Create logic node */
    t_ast_node *logic_node = create_logic_node(left_cmd, right_cmd, NODE_AND, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_logical(&logic_node->data.logic, &context, NODE_AND, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 1);
    ck_assert_int_eq(context.exit_status, 1);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 3: Logical OR with left command fails */
START_TEST(test_ms_execute_logical_or_left_fails)
{
    /* Commands: false || true */
    char *false_args[] = {"false", NULL};
    char *true_args[] = {"true", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(false_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(true_args, gcl);

    /* Create logic node */
    t_ast_node *logic_node = create_logic_node(left_cmd, right_cmd, NODE_OR, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_logical(&logic_node->data.logic, &context, NODE_OR, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_eq(context.exit_status, 0);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 4: Logical OR with left command succeeds */
START_TEST(test_ms_execute_logical_or_left_succeeds)
{
    /* Commands: true || false */
    char *true_args[] = {"true", NULL};
    char *false_args[] = {"false", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(true_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(false_args, gcl);

    /* Create logic node */
    t_ast_node *logic_node = create_logic_node(left_cmd, right_cmd, NODE_OR, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_logical(&logic_node->data.logic, &context, NODE_OR, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_eq(context.exit_status, 0);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 5: Left and right commands both fail with logical AND */
START_TEST(test_ms_execute_logical_and_both_fail)
{
    /* Commands: false && false */
    char *false_args[] = {"false", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(false_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(false_args, gcl);

    /* Create logic node */
    t_ast_node *logic_node = create_logic_node(left_cmd, right_cmd, NODE_AND, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_logical(&logic_node->data.logic, &context, NODE_AND, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 1);
    ck_assert_int_eq(context.exit_status, 1);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 6: Left and right commands both fail with logical OR */
START_TEST(test_ms_execute_logical_or_both_fail)
{
    /* Commands: false || false */
    char *false_args[] = {"false", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(false_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(false_args, gcl);

    /* Create logic node */
    t_ast_node *logic_node = create_logic_node(left_cmd, right_cmd, NODE_OR, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_logical(&logic_node->data.logic, &context, NODE_OR, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 1);
    ck_assert_int_eq(context.exit_status, 1);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 7: Invalid node type */
START_TEST(test_ms_execute_logical_invalid_type)
{
    /* Commands: true ?? true */
    char *true_args[] = {"true", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(true_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(true_args, gcl);

    /* Create logic node with invalid type */
    t_ast_node *logic_node = create_logic_node(left_cmd, right_cmd, -1, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_logical(&logic_node->data.logic, &context, -1, manager);

    /* Verify the result */
    /* Since the type is invalid, the function should default to else clause */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_eq(context.exit_status, 0);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 8: Null pointers */
START_TEST(test_ms_execute_logical_null_pointers)
{
    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function with null logic_node */
    int ret = ms_execute_logical(NULL, &context, NODE_AND, manager);

    /* Verify the result */
    /* The function may crash or handle the null pointer */
    /* For this test, we assume it returns an error code */
    ck_assert_int_eq(ret, -1);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 9: Nested logical operations */
START_TEST(test_ms_execute_logical_nested)
{
    /* Commands: (true && false) || true */
    char *true_args[] = {"true", NULL};
    char *false_args[] = {"false", NULL};

    /* Create command nodes */
    t_ast_node *cmd_true = create_cmd_node(true_args, gcl);
    t_ast_node *cmd_false = create_cmd_node(false_args, gcl);

    /* Create inner logic node: true && false */
    t_ast_node *inner_logic = create_logic_node(cmd_true, cmd_false, NODE_AND, gcl);

    /* Create outer logic node: (inner_logic) || true */
    t_ast_node *outer_logic = create_logic_node(inner_logic, cmd_true, NODE_OR, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = mock_execute_ast(outer_logic, &context, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_eq(context.exit_status, 0);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test Suite Setup */
Suite *ms_execute_logical_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_execute_logical");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_execute_logical_and_left_succeeds);
    tcase_add_test(tc_core, test_ms_execute_logical_and_left_fails);
    tcase_add_test(tc_core, test_ms_execute_logical_or_left_fails);
    tcase_add_test(tc_core, test_ms_execute_logical_or_left_succeeds);
    tcase_add_test(tc_core, test_ms_execute_logical_and_both_fail);
    tcase_add_test(tc_core, test_ms_execute_logical_or_both_fail);
    tcase_add_test(tc_core, test_ms_execute_logical_invalid_type);
    tcase_add_test(tc_core, test_ms_execute_logical_null_pointers);
    tcase_add_test(tc_core, test_ms_execute_logical_nested);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_execute_logical_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
