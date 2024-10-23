/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_syntax_error.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:00:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/23 12:54:45 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../include/syntax.h"

/* Helper function to test functions that call exit() */
int test_ms_syntax_error_exit(const char *input)
{
    int error;
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        error = ms_syntax_error(input);
        exit(error); // If exit isn't called, exit normally
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

START_TEST(test_ms_syntax_error_valid_input_0)
{
    int exit_code = test_ms_syntax_error_exit("valid input");
    ck_assert_int_eq(exit_code, 0); // Expect success with valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_1)
{
    int exit_code = test_ms_syntax_error_exit("\'valid input\'");
    ck_assert_int_eq(exit_code, 0); // Expect success with valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_2)
{
    int exit_code = test_ms_syntax_error_exit("\"valid input\"");
    ck_assert_int_eq(exit_code, 0); // Expect success with valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_3)
{
    int exit_code = test_ms_syntax_error_exit("\"valid input\"\"\"");
    ck_assert_int_eq(exit_code, 0); // Expect success with valid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_0)
{
    int exit_code = test_ms_syntax_error_exit("\"invalide input\"\'");
    ck_assert_int_eq(exit_code, 1); // Expect success with valid input
}

START_TEST(test_ms_syntax_error_invalid_input_1)
{
    int exit_code = test_ms_syntax_error_exit("\"invalide input");
    ck_assert_int_eq(exit_code, 1); // Expect success with valid input
}

START_TEST(test_ms_syntax_error_invalid_input_2)
{
    int exit_code = test_ms_syntax_error_exit("\"invalide input\'\"\'\"\'\'");
    ck_assert_int_eq(exit_code, 1); // Expect success with valid input
}

Suite *ms_syntax_error_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_syntax_error");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_0);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_1);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_2);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_3);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_0);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_1);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_2);
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
