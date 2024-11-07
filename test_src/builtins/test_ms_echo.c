/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:01:25 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 17:24:43 by jeportie         ###   ########.fr       */
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

static t_gc *g_garbage_collector;  // Déclaration globale du garbage collector

typedef struct s_testresult
{
    int exit_code;
    char stdout_output[STDOUT_BUFFER_SIZE];
    char stderr_output[STDERR_BUFFER_SIZE];
} t_testresult;

// Fonction de configuration (setup)
static void setup(void)
{
    // Initialiser le garbage collector avant chaque test
    g_garbage_collector = gc_init();
}

// Fonction de nettoyage (teardown)
static void teardown(void)
{
    // Nettoyer le garbage collector après chaque test
    gc_cleanup(g_garbage_collector);
}

static t_testresult execute_and_capture_echo(char **argv, int argc)
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
        int exit_code = ms_echo(&cmd_node);
        exit(exit_code);
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

// Test pour `echo "hello world"`
START_TEST(test_ms_echo_basic)
{
    char *argv[] = {"echo", "hello", "world", NULL};
    t_testresult result = execute_and_capture_echo(argv, 3);

    ck_assert_int_eq(result.exit_code, 0);
    ck_assert_str_eq(result.stdout_output, "hello world\n");
    ck_assert_str_eq(result.stderr_output, "");
}
END_TEST

// Test pour `echo -n "no newline"`
START_TEST(test_ms_echo_no_newline_0)
{
    char *argv[] = {"echo", "-n", "no newline", NULL};
    t_testresult result = execute_and_capture_echo(argv, 3);

    ck_assert_int_eq(result.exit_code, 0);
    ck_assert_str_eq(result.stdout_output, "no newline");
    ck_assert_str_eq(result.stderr_output, "");
}
END_TEST

START_TEST(test_ms_echo_no_newline_1)
{
    char *argv[] = {"echo", "-nnnnnnnnnnn", "no newline", NULL};
    t_testresult result = execute_and_capture_echo(argv, 3);

    ck_assert_int_eq(result.exit_code, 0);
    ck_assert_str_eq(result.stdout_output, "no newline");
    ck_assert_str_eq(result.stderr_output, "");
}
END_TEST

START_TEST(test_ms_echo_no_newline_2)
{
    char *argv[] = {"echo", "-n", "-n", "no newline", NULL};
    t_testresult result = execute_and_capture_echo(argv, 3);

    ck_assert_int_eq(result.exit_code, 0);
    ck_assert_str_eq(result.stdout_output, "no newline");
    ck_assert_str_eq(result.stderr_output, "");
}
END_TEST

START_TEST(test_ms_echo_no_newline_3)
{
    char *argv[] = {"echo", "-nnnnn-n", "newline", NULL};
    t_testresult result = execute_and_capture_echo(argv, 3);

    ck_assert_int_eq(result.exit_code, 0);
    ck_assert_str_eq(result.stdout_output, "-nnnnn-n newline\n");
    ck_assert_str_eq(result.stderr_output, "");
}
END_TEST

static Suite *ms_echo_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_echo");
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);

    tcase_add_test(tc_core, test_ms_echo_basic);
    tcase_add_test(tc_core, test_ms_echo_no_newline_0);
    tcase_add_test(tc_core, test_ms_echo_no_newline_1);
    tcase_add_test(tc_core, test_ms_echo_no_newline_2);
    tcase_add_test(tc_core, test_ms_echo_no_newline_3);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_echo_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
