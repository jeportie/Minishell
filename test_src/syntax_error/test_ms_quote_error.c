/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_quote_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:21:30 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 15:29:22 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../include/syntax.h"

int test_ms_quote_error_exit(const char *input, t_shell *shell)
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        ms_quote_error(input, shell);
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

/* Mock shell structure for testing */
t_shell* create_mock_shell(void)
{
    t_shell *shell = malloc(sizeof(t_shell));
    shell->gcl = gc_init();
    return shell;
}

void free_mock_shell(t_shell *shell)
{
    free(shell->gcl);
    free(shell);
}

START_TEST(test_ms_quote_error_null_input)
{
    t_shell *shell = create_mock_shell();
    int exit_code = test_ms_quote_error_exit(NULL, shell);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to null input
}
END_TEST

START_TEST(test_ms_quote_error_no_shell)
{
    int exit_code = test_ms_quote_error_exit("\"valide input\"", NULL);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to null input
}

START_TEST(test_ms_quote_error_null_input_no_shell)
{
    int exit_code = test_ms_quote_error_exit(NULL, NULL);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to null input
}

START_TEST(test_ms_quote_error_invalid_0_input)
{
    t_shell *shell = create_mock_shell();
    char    *input = "\"invalid input";
    int exit_code = test_ms_quote_error_exit(input, shell);
    ck_assert_int_eq(exit_code, EXIT_FAILURE); // Expect failure due to null input
}
END_TEST

START_TEST(test_ms_quote_error_valid_0_input)
{
    t_shell *shell = create_mock_shell();
    char    *input = "\"valid input\"";
    int exit_code = test_ms_quote_error_exit(input, shell);
    ck_assert_int_eq(exit_code, EXIT_SUCCESS); // Expect failure due to null input
}
END_TEST

Suite *ms_quote_error_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_quote_error");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_quote_error_null_input);
    tcase_add_test(tc_core, test_ms_quote_error_no_shell);
    tcase_add_test(tc_core, test_ms_quote_error_null_input_no_shell);
    tcase_add_test(tc_core, test_ms_quote_error_invalid_0_input);
    tcase_add_test(tc_core, test_ms_quote_error_valid_0_input);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_quote_error_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

