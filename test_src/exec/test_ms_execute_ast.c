/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_execute_ast.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:36:07 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 14:07:47 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/exec.h"
#include "../../include/ast.h"
#include "../../include/minishell.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

static t_gc *gcl;

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

/* Helper Functions */

/* Create a command node for testing */
t_ast_node *create_command_node_for_test(char **args)
{
    t_ast_node *node = gc_malloc(sizeof(t_ast_node), gcl);
    if (!node)
    {
        perror("gc_malloc");
        exit(EXIT_FAILURE);
    }

    node->type = NODE_COMMAND;
    node->data.command.argc = 0;
    node->data.command.argv = args;
    node->data.command.is_expand = false;

    /* Count arguments */
    while (args[node->data.command.argc])
        node->data.command.argc++;

    return node;
}

/* Initialize execution context for testing */
t_exec_context initialize_exec_context_for_test()
{
    t_exec_context context;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.shell = gc_malloc(sizeof(t_shell), gcl);
    if (!context.shell)
    {
        perror("gc_malloc");
        exit(EXIT_FAILURE);
    }
    context.shell->gcl = gcl;
    context.shell->env_data = gc_malloc(sizeof(t_env_data), gcl);
    if (!context.shell->env_data)
    {
        perror("gc_malloc");
        exit(EXIT_FAILURE);
    }
    context.shell->env_data->env = NULL;
    context.shell->env_data->path = NULL;
    context.is_subprocess = false;
    context.exit_status = 0;
    context.child_lvl = 0;

    /* Set environment variables for testing */
    extern char **environ;
    context.shell->env_data = ms_init_env(environ, context.shell);

    /* Get PATH from environ */
    char *path_env = getenv("PATH");
    if (path_env)
    {
        context.shell->env_data->path = path_env;
    }
    else
    {
        context.shell->env_data->path = "/bin:/usr/bin";
    }

    return context;
}

/* Test Cases */

/* Test 1: Null AST Node */
START_TEST(test_ms_execute_ast_null_node)
{
    t_exec_context context = initialize_exec_context_for_test();
    t_proc_manager *manager = init_manager(gcl);

    int result = ms_execute_ast(NULL, &context, manager);

    ck_assert_int_eq(result, -1); // Assuming -1 indicates an error

    clean_manager(manager);
}
END_TEST

/* Test 2: Unsupported Node Type */
START_TEST(test_ms_execute_ast_unsupported_node)
{
    t_ast_node *node = gc_malloc(sizeof(t_ast_node), gcl);
    if (!node)
    {
        perror("gc_malloc");
        exit(EXIT_FAILURE);
    }
    node->type = ERR; // Assuming ERR is an unsupported node type

    t_exec_context context = initialize_exec_context_for_test();
    t_proc_manager *manager = init_manager(gcl);

    int result = ms_execute_ast(node, &context, manager);

    ck_assert_int_eq(result, -1);

    clean_manager(manager);
}
END_TEST

/* Test 3: Simple Command Execution */
START_TEST(test_ms_execute_ast_simple_command)
{
    char *args[] = {"echo", "Hello World", NULL};
    t_ast_node *node = create_command_node_for_test(args);

    t_exec_context context = initialize_exec_context_for_test();
    t_proc_manager *manager = init_manager(gcl);

    /* Redirect stdout to a pipe */
    int pipefd[2];
    ck_assert_int_eq(pipe(pipefd), 0);

    pid_t pid = fork();
    ck_assert_int_ne(pid, -1);

    if (pid == 0)
    {
        /* Child process */
        close(pipefd[0]); /* Close read end */
        dup2(pipefd[1], STDOUT_FILENO); /* Redirect stdout to pipe */
        close(pipefd[1]);

        int ret = ms_execute_ast(node, &context, manager);
        exit(ret);
    }
    else
    {
        /* Parent process */
        close(pipefd[1]); /* Close write end */

        /* Read output from pipe */
        char buffer[1024];
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        close(pipefd[0]);

        if (bytes_read >= 0)
        {
            buffer[bytes_read] = '\0';
            /* Remove trailing newline for comparison */
            char *newline = strchr(buffer, '\n');
            if (newline)
                *newline = '\0';
            ck_assert_str_eq(buffer, "Hello World");
        }
        else
        {
            ck_abort_msg("Failed to read from pipe");
        }

        int status;
        waitpid(pid, &status, 0);
        ck_assert_int_eq(WEXITSTATUS(status), 0);
    }

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 4: Pipeline Execution */
START_TEST(test_ms_execute_ast_pipeline)
{
    /* Create AST nodes for "echo 'Hello World' | grep Hello" */
    char *args1[] = {"echo", "Hello World", NULL};
    t_ast_node *left_node = create_command_node_for_test(args1);

    char *args2[] = {"grep", "Hello", NULL};
    t_ast_node *right_node = create_command_node_for_test(args2);

    t_ast_node *pipeline_node = gc_malloc(sizeof(t_ast_node), gcl);
    if (!pipeline_node)
    {
        perror("gc_malloc");
        exit(EXIT_FAILURE);
    }
    pipeline_node->type = NODE_PIPE;
    pipeline_node->data.pipe.left = left_node;
    pipeline_node->data.pipe.right = right_node;

    t_exec_context context = initialize_exec_context_for_test();
    t_proc_manager *manager = init_manager(gcl);

    /* Redirect stdout to a pipe */
    int pipefd[2];
    ck_assert_int_eq(pipe(pipefd), 0);

    pid_t pid = fork();
    ck_assert_int_ne(pid, -1);

    if (pid == 0)
    {
        /* Child process */
        close(pipefd[0]); /* Close read end */
        dup2(pipefd[1], STDOUT_FILENO); /* Redirect stdout to pipe */
        close(pipefd[1]);

        int ret = ms_execute_ast(pipeline_node, &context, manager);
        exit(ret);
    }
    else
    {
        /* Parent process */
        close(pipefd[1]); /* Close write end */

        /* Read output from pipe */
        char buffer[1024];
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        close(pipefd[0]);

        if (bytes_read >= 0)
        {
            buffer[bytes_read] = '\0';
            /* Remove trailing newline for comparison */
            char *newline = strchr(buffer, '\n');
            if (newline)
                *newline = '\0';
            ck_assert_str_eq(buffer, "Hello World");
        }
        else
        {
            ck_abort_msg("Failed to read from pipe");
        }

        int status;
        waitpid(pid, &status, 0);
        ck_assert_int_eq(WEXITSTATUS(status), 0);
    }

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 5: Logical AND Execution */
START_TEST(test_ms_execute_ast_logical_and)
{
    /* Create AST nodes for "true && echo 'Success'" */
    char *args1[] = {"true", NULL};
    t_ast_node *left_node = create_command_node_for_test(args1);

    char *args2[] = {"echo", "Success", NULL};
    t_ast_node *right_node = create_command_node_for_test(args2);

    t_ast_node *and_node = gc_malloc(sizeof(t_ast_node), gcl);
    if (!and_node)
    {
        perror("gc_malloc");
        exit(EXIT_FAILURE);
    }
    and_node->type = NODE_AND;
    and_node->data.logic.left = left_node;
    and_node->data.logic.right = right_node;

    t_exec_context context = initialize_exec_context_for_test();
    t_proc_manager *manager = init_manager(gcl);

    /* Redirect stdout to a pipe */
    int pipefd[2];
    ck_assert_int_eq(pipe(pipefd), 0);

    pid_t pid = fork();
    ck_assert_int_ne(pid, -1);

    if (pid == 0)
    {
        /* Child process */
        close(pipefd[0]); /* Close read end */
        dup2(pipefd[1], STDOUT_FILENO); /* Redirect stdout to pipe */
        close(pipefd[1]);

        int ret = ms_execute_ast(and_node, &context, manager);
        exit(ret);
    }
    else
    {
        /* Parent process */
        close(pipefd[1]); /* Close write end */

        /* Read output from pipe */
        char buffer[1024];
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        close(pipefd[0]);

        if (bytes_read >= 0)
        {
            buffer[bytes_read] = '\0';
            /* Remove trailing newline for comparison */
            char *newline = strchr(buffer, '\n');
            if (newline)
                *newline = '\0';
            ck_assert_str_eq(buffer, "Success");
        }
        else
        {
            ck_abort_msg("Failed to read from pipe");
        }

        int status;
        waitpid(pid, &status, 0);
        ck_assert_int_eq(WEXITSTATUS(status), 0);
    }

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 6: Redirection Output */
START_TEST(test_ms_execute_ast_redirection_output)
{
    /* Create AST nodes for "echo 'Hello World' > output.txt" */
    char *args[] = {"echo", "Hello World", NULL};
    t_ast_node *command_node = create_command_node_for_test(args);

    t_ast_node *redirect_node = gc_malloc(sizeof(t_ast_node), gcl);
    if (!redirect_node)
    {
        perror("gc_malloc");
        exit(EXIT_FAILURE);
    }
    redirect_node->type = NODE_REDIRECT_OUT;
    redirect_node->data.redirect.child = command_node;
    redirect_node->data.redirect.filename = "output.txt"; // Assuming filename is managed by gc

    t_exec_context context = initialize_exec_context_for_test();
    t_proc_manager *manager = init_manager(gcl);

    pid_t pid = fork();
    ck_assert_int_ne(pid, -1);

    if (pid == 0)
    {
        /* Child process */
        int ret = ms_execute_ast(redirect_node, &context, manager);
        exit(ret);
    }
    else
    {
        /* Parent process */
        int status;
        waitpid(pid, &status, 0);
        ck_assert_int_eq(WEXITSTATUS(status), 0);

        /* Check the content of output.txt */
        FILE *file = fopen("output.txt", "r");
        ck_assert_ptr_nonnull(file);

        char buffer[1024];
        char *line = fgets(buffer, sizeof(buffer), file);
        fclose(file);

        /* Remove the output file after reading */
        remove("output.txt");

        ck_assert_ptr_nonnull(line);
        /* Remove trailing newline for comparison */
        char *newline = strchr(buffer, '\n');
        if (newline)
            *newline = '\0';
        ck_assert_str_eq(buffer, "Hello World");
    }

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test Suite Setup */
Suite *ms_execute_ast_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_execute_ast");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_execute_ast_null_node);
    tcase_add_test(tc_core, test_ms_execute_ast_unsupported_node);
    tcase_add_test(tc_core, test_ms_execute_ast_simple_command);
    tcase_add_test(tc_core, test_ms_execute_ast_pipeline);
    tcase_add_test(tc_core, test_ms_execute_ast_logical_and);
    tcase_add_test(tc_core, test_ms_execute_ast_redirection_output);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_execute_ast_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
