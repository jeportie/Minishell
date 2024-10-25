/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 22:49:51 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/25 22:56:52 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "run_minishell_command.h"

// Assume run_minishell_command is defined as above

START_TEST(test_echo_simple)
{
	const char	*exec_path = "./minishell";
    char *input = "echo Hello, World!";
    char *expected_output = "Hello, World!\n";
    char *actual_output = NULL;
    int exit_code;

    exit_code = run_minishell_command(exec_path, input, &actual_output);

    // Check exit code
    ck_assert_int_eq(exit_code, 0);

    // Check output
    // Note: You may need to adjust expected_output to account for any prompts or extra output from Minishell
    ck_assert_str_eq(actual_output, expected_output);

    // Clean up
    free(actual_output);
}
END_TEST

Suite *minishell_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("Minishell");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_echo_simple);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void)
{
	int no_failed = 0;
	Suite *s;
	SRunner *runner;

	s = minishell_suite();
	runner = srunner_create(s);

	srunner_run_all(runner, CK_NORMAL);
	no_failed = srunner_ntests_failed(runner);
	srunner_free(runner);

	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
