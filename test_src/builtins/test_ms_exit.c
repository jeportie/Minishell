/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:50:52 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/05 21:17:19 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../../include/builtins.h"

#define STDERR_BUFFER_SIZE 1024
#define STDOUT_BUFFER_SIZE 1024

static t_gc *g_garbage_collector;

typedef struct s_testresult
{
    int exit_code;
    char stdout_output[STDOUT_BUFFER_SIZE];
    char stderr_output[STDERR_BUFFER_SIZE];
} t_testresult;

static void setup(void)
{
    g_garbage_collector = gc_init();
}

static void teardown(void)
{
    gc_cleanup(g_garbage_collector);
}

static t_testresult execute_and_capture_exit(char **argv, int argc, int expected_exit_code)
{
    t_testresult result;
    int stdout_pipefd[2];
    int stderr_pipefd[2];
    pid_t pid;
    int status;
    ssize_t bytes_read;

    memset(result.stdout_output, 0, STDOUT_BUFFER_SIZE);
    memset(result.stderr_output, 0, STDERR_BUFFER_SIZE);

    if (pipe(stdout_pipefd) == -1 || pipe(stderr_pipefd) == -1)
    {
        perror("Erreur lors de la création des pipes");
        result.exit_code = -1;
        strncpy(result.stderr_output, "Erreur lors de la création des pipes\n", STDERR_BUFFER_SIZE - 1);
        return result;
    }

    gc_fd_register(stdout_pipefd[0], g_garbage_collector);
    gc_fd_register(stdout_pipefd[1], g_garbage_collector);
    gc_fd_register(stderr_pipefd[0], g_garbage_collector);
    gc_fd_register(stderr_pipefd[1], g_garbage_collector);

    pid = fork();
    if (pid == -1)
    {
        perror("Erreur lors du fork");
        result.exit_code = -1;
        strncpy(result.stderr_output, "Erreur lors du fork\n", STDERR_BUFFER_SIZE - 1);
        return result;
    }
    else if (pid == 0)
    {
        close(stdout_pipefd[0]);
        close(stderr_pipefd[0]);

        if (dup2(stdout_pipefd[1], STDOUT_FILENO) == -1 || dup2(stderr_pipefd[1], STDERR_FILENO) == -1)
        {
            perror("Erreur lors de dup2");
            exit(EXIT_FAILURE);
        }

        close(stdout_pipefd[1]);
        close(stderr_pipefd[1]);

        t_cmd_node cmd_node = {.argv = argv, .argc = argc};
        t_shell shell = {.error_code = expected_exit_code, .gcl = g_garbage_collector};
        t_exec_context context = {.shell = &shell};
        ms_exit(&cmd_node, &context);
        exit(expected_exit_code);
    }
    else
    {
        close(stdout_pipefd[1]);
        close(stderr_pipefd[1]);

        waitpid(pid, &status, 0);
        result.exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

        bytes_read = read(stdout_pipefd[0], result.stdout_output, STDOUT_BUFFER_SIZE - 1);
        result.stdout_output[bytes_read >= 0 ? bytes_read : 0] = '\0';

        bytes_read = read(stderr_pipefd[0], result.stderr_output, STDERR_BUFFER_SIZE - 1);
        result.stderr_output[bytes_read >= 0 ? bytes_read : 0] = '\0';

        close(stdout_pipefd[0]);
        close(stderr_pipefd[0]);

        return result;
    }
}

// Test `exit` avec un code de sortie valide
START_TEST(test_ms_exit_with_valid_code)
{
    char *argv[] = {"exit", "42", NULL};
    t_testresult result = execute_and_capture_exit(argv, 2, 42);

    ck_assert_int_eq(result.exit_code, 42);
    ck_assert_str_eq(result.stderr_output, "exit\n");
}
END_TEST

// Test `exit` avec un code de sortie invalide (non numérique)
START_TEST(test_ms_exit_with_invalid_code)
{
    char *argv[] = {"exit", "invalid", NULL};
    t_testresult result = execute_and_capture_exit(argv, 2, 2);

    ck_assert_int_eq(result.exit_code, 2);
    ck_assert_str_eq(result.stderr_output, "exit\nminishell: exit: invalid: numeric argument required\n");
}
END_TEST

// Test `exit` avec trop d'arguments
START_TEST(test_ms_exit_with_too_many_arguments)
{
    char *argv[] = {"exit", "1", "extra_arg", NULL};
    t_testresult result = execute_and_capture_exit(argv, 3, 1);

    ck_assert_int_eq(result.exit_code, 1);
    ck_assert_str_eq(result.stderr_output, "exit\nminishell: exit: too many arguments\n");
}
END_TEST

static Suite *ms_exit_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_exit");
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);

    tcase_add_test(tc_core, test_ms_exit_with_valid_code);
    tcase_add_test(tc_core, test_ms_exit_with_invalid_code);
    tcase_add_test(tc_core, test_ms_exit_with_too_many_arguments);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_exit_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

