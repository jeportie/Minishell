/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_execute_subshell.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:09:06 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 13:28:49 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/exec.h"
#include "../../include/process.h"
#include "../../include/minishell.h"
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
        else if (strcmp(cmd_node->argv[0], "exit") == 0 && cmd_node->argv[1])
        {
            return atoi(cmd_node->argv[1]); /* Return specified exit code */
        }
        else
        {
            return 127; /* Command not found */
        }
    }
    else if (node->type == NODE_SUBSHELL)
    {
        /* Subshell node */
        return ms_execute_subshell(&node->data.subshell, context, manager);
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

/* Helper function to create a subshell node */
t_ast_node *create_mocksubshell_node(t_ast_node *child, t_gc *gcl)
{
    t_ast_node *node = gc_malloc(sizeof(t_ast_node), gcl);
    node->type = NODE_SUBSHELL;
    node->data.subshell.child = child;
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

/* Test 1: Subshell executes successfully */
START_TEST(test_ms_execute_subshell_success)
{
    /* Command: (true) */
    char *true_args[] = {"true", NULL};

    /* Create command node */
    t_ast_node *cmd_node = create_cmd_node(true_args, gcl);

    /* Create subshell node */
    t_ast_node *subshell_node = create_mocksubshell_node(cmd_node, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;
    context.child_lvl = 0;
    context.is_subprocess = false;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_subshell(&subshell_node->data.subshell, &context, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_eq(context.exit_status, 0);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 2: Subshell command fails */
START_TEST(test_ms_execute_subshell_failure)
{
    /* Command: (false) */
    char *false_args[] = {"false", NULL};

    /* Create command node */
    t_ast_node *cmd_node = create_cmd_node(false_args, gcl);

    /* Create subshell node */
    t_ast_node *subshell_node = create_mocksubshell_node(cmd_node, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;
    context.child_lvl = 0;
    context.is_subprocess = false;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_subshell(&subshell_node->data.subshell, &context, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 1);
    ck_assert_int_eq(context.exit_status, 1);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 3: Subshell with exit command */
START_TEST(test_ms_execute_subshell_exit_code)
{
    /* Command: (exit 42) */
    char *exit_args[] = {"exit", "42", NULL};

    /* Create command node */
    t_ast_node *cmd_node = create_cmd_node(exit_args, gcl);

    /* Create subshell node */
    t_ast_node *subshell_node = create_mocksubshell_node(cmd_node, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;
    context.child_lvl = 0;
    context.is_subprocess = false;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_subshell(&subshell_node->data.subshell, &context, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 42);
    ck_assert_int_eq(context.exit_status, 42);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 4: Subshell with invalid command */
START_TEST(test_ms_execute_subshell_invalid_command)
{
    /* Command: (invalidcmd) */
    char *invalid_args[] = {"invalidcmd", NULL};

    /* Create command node */
    t_ast_node *cmd_node = create_cmd_node(invalid_args, gcl);

    /* Create subshell node */
    t_ast_node *subshell_node = create_mocksubshell_node(cmd_node, gcl);

    /* Initialize execution context */
    t_exec_context context;
    context.exit_status = 0;
    context.child_lvl = 0;
    context.is_subprocess = false;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_subshell(&subshell_node->data.subshell, &context, manager);

    /* Verify the result */
    ck_assert_int_eq(ret, 127);
    ck_assert_int_eq(context.exit_status, 127);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

Suite *ms_execute_subshell_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_execute_subshell");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_execute_subshell_success);
    tcase_add_test(tc_core, test_ms_execute_subshell_failure);
    tcase_add_test(tc_core, test_ms_execute_subshell_exit_code);
    tcase_add_test(tc_core, test_ms_execute_subshell_invalid_command);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_execute_subshell_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
