/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_syntax_error.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:00:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 13:11:04 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../include/syntax.h"

/* Helper function to test functions that call exit() */
int test_ms_syntax_error_exit(const char *input, t_shell *shell)
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        ms_syntax_error(input, shell);
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

START_TEST(test_ms_syntax_error_null_input)
{
    t_shell shell;
    shell.gcl = gc_init(); // Simulate garbage collector setup
    int exit_code = test_ms_syntax_error_exit(NULL, &shell);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to null input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input)
{
    t_shell shell;
    shell.gcl = gc_init(); // Simulate garbage collector setup
    int exit_code = test_ms_syntax_error_exit("valid input", &shell);
    ck_assert_int_eq(exit_code, EXIT_SUCCESS); // Expect success with valid input
}
END_TEST

START_TEST(test_ms_syntax_error_no_shell)
{
    int exit_code = test_ms_syntax_error_exit("valid input", NULL);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to null shell pointer
}
END_TEST

Suite *ms_syntax_error_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_syntax_error");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_syntax_error_null_input);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input);
    tcase_add_test(tc_core, test_ms_syntax_error_no_shell);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_syntax_error_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
