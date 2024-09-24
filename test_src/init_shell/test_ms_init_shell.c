/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_init_shell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:43:18 by jeportie          #+#    #+#             */
/*   Updated: 2024/09/24 16:43:50 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <check.h>

START_TEST(test_ms_init_shell_initialization)
{
	int argc = 0;
	char **envp = NULL;

	t_shell shell = ms_init_shell(argc, envp);
	ck_assert_ptr_null(shell.user_input);
	ck_assert_ptr_null(shell.error_code);
	// Add other assertions as needed
}
END_TEST

Suite *minishell_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Minishell");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_init_shell_initialization);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = minishell_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
