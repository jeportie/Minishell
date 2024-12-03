/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_init_shell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:43:18 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/27 12:26:43 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../include/minishell.h"

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

START_TEST(test_ms_init_shell_no_args)
{
    char *argv[] = {"minishell", NULL};
    int exit_code = test_ms_init_shell_exit(1, argv, NULL);

    ck_assert_int_eq(exit_code, 1);
}
END_TEST

START_TEST(test_ms_init_shell_with_invalid_argc)
{
    char *argv[] = {"minishell", "arg1", NULL};
    int exit_code = test_ms_init_shell_exit(2, argv, NULL);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to extra argument
}
END_TEST

START_TEST(test_ms_init_shell_with_envp)
{
    char *envp[] = {"PATH=/usr/bin", "HOME=/home/user", NULL};
    char *argv[] = {"minishell", NULL};
    int exit_code = test_ms_init_shell_exit(1, argv, envp);
    ck_assert_int_eq(exit_code, EXIT_SUCCESS); // Expect success
}
END_TEST

START_TEST(test_ms_init_shell_with_empty_envp)
{
    char *envp[] = {NULL};
    char *argv[] = {"minishell", NULL};
    int exit_code = test_ms_init_shell_exit(1, argv, envp);
    ck_assert_int_eq(exit_code, EXIT_SUCCESS); // Expect success with default environment
}
END_TEST

Suite *ms_init_shell_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_init_shell");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_init_shell_no_args);
    tcase_add_test(tc_core, test_ms_init_shell_with_invalid_argc);
    tcase_add_test(tc_core, test_ms_init_shell_with_envp);
    tcase_add_test(tc_core, test_ms_init_shell_with_empty_envp);
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
