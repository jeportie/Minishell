/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_get_user_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:21:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/09/24 21:00:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/minishell.h"

/**
 * Mock readline function
 */
char	*mock_readline(const char *prompt)
{
	(void)prompt;
	return (strdup("test input"));
}

/**
 * Mock add_history function
 */
void	mock_add_history(const char *line)
{
	(void)line;
}

/**
 * Test function for normal input
 */
START_TEST(test_ms_get_user_input_normal)
{
	t_shell	shell;
	shell.user_input = NULL;
	shell.user_input = mock_readline("ptit'coque> ");
	mock_add_history(shell.user_input);
	ck_assert_ptr_nonnull(shell.user_input);
	ck_assert_str_eq(shell.user_input, "test input");
	free(shell.user_input);
}
END_TEST

/**
 * Test function for empty input
 */
START_TEST(test_ms_get_user_input_empty)
{
	t_shell	shell;

	shell.user_input = NULL;
	shell.user_input = strdup("");
	mock_add_history(shell.user_input);
	ck_assert_ptr_nonnull(shell.user_input);
	ck_assert_str_eq(shell.user_input, "");
	free(shell.user_input);
}
END_TEST

/**
 * Test function for special characters
 */
START_TEST(test_ms_get_user_input_special_chars)
{
	t_shell	shell;

	shell.user_input = NULL;
	shell.user_input = strdup("!@#$%^&*()");
	mock_add_history(shell.user_input);
	ck_assert_ptr_nonnull(shell.user_input);
	ck_assert_str_eq(shell.user_input, "!@#$%^&*()");
	free(shell.user_input);
}
END_TEST

/**
 * Creates test suite for minishell
 */
Suite	*minishell_suite(void)
{
	Suite	*s;
	TCase	*tc_core;

	s = suite_create("Minishell");
	tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_ms_get_user_input_normal);
	tcase_add_test(tc_core, test_ms_get_user_input_empty);
	tcase_add_test(tc_core, test_ms_get_user_input_special_chars);
	suite_add_tcase(s, tc_core);
	return (s);
}

int	main(void)
{
	int		number_failed;
	Suite	*s;
	SRunner	*sr;

	s = minishell_suite();
	sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
