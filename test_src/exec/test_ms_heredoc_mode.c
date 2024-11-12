/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_heredoc_mode.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: assistant <assistant@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 16:00:00 by assistant         #+#    #+#             */
/*   Updated: 2024/11/12 12:36:52 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/exec.h"
#include "../../include/process.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

static t_gc *gcl;

/* Helper function to simulate user input */
void write_to_stdin_pipe(int write_fd, const char *input)
{
    write(write_fd, input, strlen(input));
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

/* Test 1: Normal operation */
START_TEST(test_ms_heredoc_mode_normal)
{
    const char *delimiter = "EOF";
    t_exec_context context;
    t_proc_manager *manager;
    int stdin_pipe[2];
    int stdout_pipe[2];

    /* Initialize context */
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    context.shell = NULL;

    /* Initialize process manager */
    manager = init_manager(gcl);

    /* Create pipes to simulate stdin and stdout */
    ck_assert_int_eq(pipe(stdin_pipe), 0);
    ck_assert_int_eq(pipe(stdout_pipe), 0);

    /* Redirect STDIN and STDOUT */
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    ck_assert_int_eq(dup2(stdin_pipe[0], STDIN_FILENO), STDIN_FILENO);
    ck_assert_int_eq(dup2(stdout_pipe[1], STDOUT_FILENO), STDOUT_FILENO);

    /* Simulate user input */
    const char *input = "line 1\nline 2\nEOF\n";
    write_to_stdin_pipe(stdin_pipe[1], input);

    /* Close write end of stdin pipe */
    close(stdin_pipe[1]);

    /* Call the function */
    int ret = ms_heredoc_mode(delimiter, &context, manager, gcl);

    /* Restore STDIN and STDOUT */
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);

    /* Close unused pipe ends */
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);

    /* Read the heredoc content from context.stdin_fd */
    char buffer[1024];
    ssize_t bytes_read = read(context.stdin_fd, buffer, sizeof(buffer) - 1);
    ck_assert_int_gt(bytes_read, 0);
    buffer[bytes_read] = '\0';

    /* Verify the content */
    ck_assert_str_eq(buffer, "line 1\nline 2\n");

    /* Clean up */
    close(context.stdin_fd);
    close(stdout_pipe[0]);
    clean_manager(manager);
}
END_TEST

/* Test 2: Immediate delimiter */
START_TEST(test_ms_heredoc_mode_immediate_delimiter)
{
    const char *delimiter = "EOF";
    t_exec_context context;
    t_proc_manager *manager;
    int stdin_pipe[2];
    int stdout_pipe[2];

    /* Initialize context */
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    context.shell = NULL;

    /* Initialize process manager */
    manager = init_manager(gcl);

    /* Create pipes to simulate stdin and stdout */
    ck_assert_int_eq(pipe(stdin_pipe), 0);
    ck_assert_int_eq(pipe(stdout_pipe), 0);

    /* Redirect STDIN and STDOUT */
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    ck_assert_int_eq(dup2(stdin_pipe[0], STDIN_FILENO), STDIN_FILENO);
    ck_assert_int_eq(dup2(stdout_pipe[1], STDOUT_FILENO), STDOUT_FILENO);

    /* Simulate user input */
    const char *input = "EOF\n";
    write_to_stdin_pipe(stdin_pipe[1], input);

    /* Close write end of stdin pipe */
    close(stdin_pipe[1]);

    /* Call the function */
    int ret = ms_heredoc_mode(delimiter, &context, manager, gcl);

    /* Restore STDIN and STDOUT */
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);

    /* Close unused pipe ends */
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);

    /* Read the heredoc content from context.stdin_fd */
    char buffer[1024];
    ssize_t bytes_read = read(context.stdin_fd, buffer, sizeof(buffer) - 1);
    /* Expecting no data */
    ck_assert_int_eq(bytes_read, 0);

    /* Clean up */
    close(context.stdin_fd);
    close(stdout_pipe[0]);
    clean_manager(manager);
}
END_TEST

/* Test 3: User interrupt (Ctrl+C) */
START_TEST(test_ms_heredoc_mode_interrupt)
{
    const char *delimiter = "EOF";
    t_exec_context context;
    t_proc_manager *manager;
    int stdin_pipe[2];
    int stdout_pipe[2];

    /* Initialize context */
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    context.shell = NULL;

    /* Initialize process manager */
    manager = init_manager(gcl);

    /* Create pipes to simulate stdin and stdout */
    ck_assert_int_eq(pipe(stdin_pipe), 0);
    ck_assert_int_eq(pipe(stdout_pipe), 0);

    /* Redirect STDIN and STDOUT */
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    ck_assert_int_eq(dup2(stdin_pipe[0], STDIN_FILENO), STDIN_FILENO);
    ck_assert_int_eq(dup2(stdout_pipe[1], STDOUT_FILENO), STDOUT_FILENO);

    /* Fork to simulate user sending Ctrl+C */
    pid_t pid = fork();
    ck_assert_int_ne(pid, -1);

    if (pid == 0)
    {
        /* Child process: simulate user input and send SIGINT */
        sleep(1); /* Wait for heredoc to prompt */
        kill(getppid(), SIGINT);
        exit(0);
    }
    else
    {
        /* Parent process: call ms_heredoc_mode */
        int ret = ms_heredoc_mode(delimiter, &context, manager, gcl);

        /* Restore STDIN and STDOUT */
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);

        /* Close unused pipe ends */
        close(stdin_pipe[0]);
        close(stdin_pipe[1]);
        close(stdout_pipe[1]);

        /* Wait for the child process */
        waitpid(pid, NULL, 0);

        /* Verify the result */
        ck_assert_int_eq(ret, -1);
        ck_assert_int_eq(context.exit_status, 130); /* 128 + SIGINT */

        /* Clean up */
        close(stdout_pipe[0]);
        clean_manager(manager);
    }
}
END_TEST

/* Additional tests can be added to cover other edge cases */

/* Test Suite Setup */
Suite *ms_heredoc_mode_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_heredoc_mode");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_heredoc_mode_normal);
    tcase_add_test(tc_core, test_ms_heredoc_mode_immediate_delimiter);
    tcase_add_test(tc_core, test_ms_heredoc_mode_interrupt);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_heredoc_mode_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
