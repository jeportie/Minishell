/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_init_shell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:43:18 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/01 13:14:29 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../include/minishell.h"

/* Helper function to test functions that call exit() */
int test_ms_init_shell_exit(int argc, char **argv, char **envp)
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        ms_init_shell(argc, argv, envp);
        exit(EXIT_SUCCESS); // If exit isn't called, exit normally
    }
    else if (pid > 0) // Parent process
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status); // Return the exit code
        }
        else
        {
            return -1; // Error occurred
        }
    }
    return -1; // Fork failed
}

START_TEST(test_ms_init_shell_basic)
{
    char *argv[] = {NULL};
    int exit_code = test_ms_init_shell_exit(0, argv, NULL);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to invalid argc/argv
}
END_TEST

START_TEST(test_ms_init_shell_with_envp)
{
    char *envp[] = {"PATH=/usr/bin", "HOME=/home/user", NULL};
    char *argv[] = {NULL};
    int exit_code = test_ms_init_shell_exit(2, argv, envp);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to invalid argc/argv
}
END_TEST

START_TEST(test_ms_init_shell_with_argc)
{
    char *argv[] = {NULL};
    int exit_code = test_ms_init_shell_exit(5, argv, NULL);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to invalid argc
}
END_TEST

START_TEST(test_ms_init_shell_with_large_argc)
{
    char *argv[] = {NULL};
    int exit_code = test_ms_init_shell_exit(1000, argv, NULL);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to large argc
}
END_TEST

START_TEST(test_ms_init_shell_with_empty_envp)
{
    char *envp[] = {NULL};
    char *argv[] = {NULL};
    int exit_code = test_ms_init_shell_exit(1, argv, envp);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to invalid argc
}
END_TEST

START_TEST(test_ms_init_shell_with_null_envp)
{
    char *argv[] = {NULL};
    int exit_code = test_ms_init_shell_exit(1, argv, NULL);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to invalid argc
}
END_TEST

START_TEST(test_ms_init_shell_with_special_chars_in_envp)
{
    char *envp[] = {"PATH=/usr/bin", "SPECIAL=*&^%$#@!", NULL};
    char *argv[] = {NULL};
    int exit_code = test_ms_init_shell_exit(2, argv, envp);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to invalid argc
}
END_TEST

START_TEST(test_ms_init_shell_with_long_envp)
{
    char *envp[] = {"PATH=/usr/bin", "LONG_ENV=abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", NULL};
    char *argv[] = {NULL};
    int exit_code = test_ms_init_shell_exit(2, argv, envp);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to invalid argc
}
END_TEST

START_TEST(test_ms_init_shell_with_multiple_envp)
{
    char *envp[] = {"PATH=/usr/bin", "HOME=/home/user", "SHELL=/bin/bash", "USER=test", NULL};
    char *argv[] = {NULL};
    int exit_code = test_ms_init_shell_exit(4, argv, envp);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to invalid argc
}
END_TEST

Suite *ms_init_shell_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_init_shell");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_init_shell_basic);
    tcase_add_test(tc_core, test_ms_init_shell_with_envp);
    tcase_add_test(tc_core, test_ms_init_shell_with_argc);
    tcase_add_test(tc_core, test_ms_init_shell_with_large_argc);
    tcase_add_test(tc_core, test_ms_init_shell_with_empty_envp);
    tcase_add_test(tc_core, test_ms_init_shell_with_null_envp);
    tcase_add_test(tc_core, test_ms_init_shell_with_special_chars_in_envp);
    tcase_add_test(tc_core, test_ms_init_shell_with_long_envp);
    tcase_add_test(tc_core, test_ms_init_shell_with_multiple_envp);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_init_shell_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

