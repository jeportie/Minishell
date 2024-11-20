/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_execute_pipeline.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:36:48 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/20 13:11:20 by jeportie         ###   ########.fr       */
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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

static t_gc *gcl;

/* Helper function to create a simple AST node for a command */
t_ast_node *create_cmd_node(char *command[], t_gc *gcl)
{
    t_ast_node *node = gc_malloc(sizeof(t_ast_node), gcl);
    node->type = 0; /* Command node type */
    node->data.command.argv = command;
    return node;
}


/* Mock function for ms_execute_ast */
int mock_execute_ast(t_ast_node *node, t_exec_context *context, t_proc_manager *manager)
{
    if (node->type == 0) /* Command node */
    {
        t_cmd_node *cmd_node = &node->data.command;

        /* For testing, we can simulate command execution */
        if (strcmp(cmd_node->argv[0], "echo") == 0)
        {
            /* Write arguments to stdout */
            for (int i = 1; cmd_node->argv[i]; i++)
            {
                write(context->stdout_fd, cmd_node->argv[i], strlen(cmd_node->argv[i]));
                if (cmd_node->argv[i + 1])
                    write(context->stdout_fd, " ", 1);
            }
            write(context->stdout_fd, "\n", 1);
            return 0;
        }
        else if (strcmp(cmd_node->argv[0], "cat") == 0)
        {
            /* Read from stdin and write to stdout */
            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(context->stdin_fd, buffer, sizeof(buffer))) > 0)
            {
                write(context->stdout_fd, buffer, bytes_read);
            }
            return 0;
        }
        else if (strcmp(cmd_node->argv[0], "fail") == 0)
        {
            /* Simulate a failing command */
            return 1;
        }
        else
        {
            /* Command not found */
            return 127;
        }
    }
    else
    {
        /* Other node types */
        return -1;
    }
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

/* Test 1: Valid pipeline execution */
START_TEST(test_ms_execute_pipeline_valid)
{
    /* Commands: echo "Hello World" | cat */
    char *echo_args[] = {"echo", "Hello World", NULL};
    char *cat_args[] = {"cat", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(echo_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(cat_args, gcl);

    /* Create pipe node */
    t_pipe_node pipe_node;
    pipe_node.left = left_cmd;
    pipe_node.right = right_cmd;

    /* Initialize execution context */
    t_exec_context context;
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    env_data.env = NULL;
    env_data.path = "/bin:/usr/bin";
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Redirect stdout to capture output */
    int stdout_pipe[2];
    ck_assert_int_eq(pipe(stdout_pipe), 0);
    int saved_stdout = dup(STDOUT_FILENO);
    ck_assert_int_eq(dup2(stdout_pipe[1], STDOUT_FILENO), STDOUT_FILENO);
    close(stdout_pipe[1]);

    /* Call the function */
    int ret = ms_execute_pipeline(&pipe_node, &context, manager);

    /* Restore stdout */
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    /* Read output */
    char output_buffer[256];
    ssize_t bytes_read = read(stdout_pipe[0], output_buffer, sizeof(output_buffer) - 1);
    ck_assert_int_gt(bytes_read, 0);
    output_buffer[bytes_read] = '\0';
    close(stdout_pipe[0]);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);

    /* Check output */
    /* Remove trailing newline */
    if (output_buffer[bytes_read - 1] == '\n')
        output_buffer[bytes_read - 1] = '\0';
    ck_assert_str_eq(output_buffer, "");

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 2: Left command fails */
START_TEST(test_ms_execute_pipeline_left_command_fails)
{
    /* Commands: fail | cat */
    char *fail_args[] = {"fail", NULL};
    char *cat_args[] = {"cat", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(fail_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(cat_args, gcl);

    /* Create pipe node */
    t_pipe_node pipe_node;
    pipe_node.left = left_cmd;
    pipe_node.right = right_cmd;

    /* Initialize execution context */
    t_exec_context context;
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    env_data.env = NULL;
    env_data.path = "/bin:/usr/bin";
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Redirect stdout to capture output */
    int stdout_pipe[2];
    ck_assert_int_eq(pipe(stdout_pipe), 0);
    int saved_stdout = dup(STDOUT_FILENO);
    ck_assert_int_eq(dup2(stdout_pipe[1], STDOUT_FILENO), STDOUT_FILENO);
    close(stdout_pipe[1]);

    /* Call the function */
    int ret = ms_execute_pipeline(&pipe_node, &context, manager);

    /* Restore stdout */
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    /* Read output */
    char output_buffer[256];
    ssize_t bytes_read = read(stdout_pipe[0], output_buffer, sizeof(output_buffer) - 1);
    /* Since left command fails, there should be no output */
    ck_assert_int_eq(bytes_read, 0);
    close(stdout_pipe[0]);

    /* Verify the result */
    /* The exit status should reflect the right command's exit status */
    ck_assert_int_eq(ret, 0);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 3: Right command fails */
START_TEST(test_ms_execute_pipeline_right_command_fails)
{
    /* Commands: echo "Hello World" | fail */
    char *echo_args[] = {"echo", "Hello World", NULL};
    char *fail_args[] = {"fail", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(echo_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(fail_args, gcl);

    /* Create pipe node */
    t_pipe_node pipe_node;
    pipe_node.left = left_cmd;
    pipe_node.right = right_cmd;

    /* Initialize execution context */
    t_exec_context context;
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    env_data.env = NULL;
    env_data.path = "/bin:/usr/bin";
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Redirect stdout to capture output */
    int stdout_pipe[2];
    ck_assert_int_eq(pipe(stdout_pipe), 0);
    int saved_stdout = dup(STDOUT_FILENO);
    ck_assert_int_eq(dup2(stdout_pipe[1], STDOUT_FILENO), STDOUT_FILENO);
    close(stdout_pipe[1]);

    /* Call the function */
    int ret = ms_execute_pipeline(&pipe_node, &context, manager);

    /* Restore stdout */
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    /* Read output */
    char output_buffer[256];
    ssize_t bytes_read = read(stdout_pipe[0], output_buffer, sizeof(output_buffer) - 1);
    /* Since right command fails, there should be no output */
    ck_assert_int_eq(bytes_read, 0);
    close(stdout_pipe[0]);

    /* Verify the result */
    /* The exit status should reflect the right command's failure */
    ck_assert_int_eq(ret, 127);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 4: Both commands fail */
START_TEST(test_ms_execute_pipeline_both_commands_fail)
{
    /* Commands: fail | fail */
    char *fail_args[] = {"fail", NULL};

    /* Create command nodes */
    t_ast_node *left_cmd = create_cmd_node(fail_args, gcl);
    t_ast_node *right_cmd = create_cmd_node(fail_args, gcl);

    /* Create pipe node */
    t_pipe_node pipe_node;
    pipe_node.left = left_cmd;
    pipe_node.right = right_cmd;

    /* Initialize execution context */
    t_exec_context context;
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    env_data.env = NULL;
    env_data.path = "/bin:/usr/bin";
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Redirect stdout to capture output */
    int stdout_pipe[2];
    ck_assert_int_eq(pipe(stdout_pipe), 0);
    int saved_stdout = dup(STDOUT_FILENO);
    ck_assert_int_eq(dup2(stdout_pipe[1], STDOUT_FILENO), STDOUT_FILENO);
    close(stdout_pipe[1]);

    /* Call the function */
    int ret = ms_execute_pipeline(&pipe_node, &context, manager);

    /* Restore stdout */
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    /* Read output */
    char output_buffer[256];
    ssize_t bytes_read = read(stdout_pipe[0], output_buffer, sizeof(output_buffer) - 1);
    /* Since both commands fail, there should be no output */
    ck_assert_int_eq(bytes_read, 0);
    close(stdout_pipe[0]);

    /* Verify the result */
    /* The exit status should reflect the right command's failure */
    ck_assert_int_eq(ret, 127);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Additional tests can be added to cover more edge cases */

Suite *ms_execute_pipeline_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_execute_pipeline");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_execute_pipeline_valid);
    tcase_add_test(tc_core, test_ms_execute_pipeline_left_command_fails);
    tcase_add_test(tc_core, test_ms_execute_pipeline_right_command_fails);
    tcase_add_test(tc_core, test_ms_execute_pipeline_both_commands_fail);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_execute_pipeline_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

