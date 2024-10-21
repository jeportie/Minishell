/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_operator_error.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:22:13 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 13:26:34 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../include/syntax.h"

/* Mock shell structure for testing */
t_shell *create_mock_shell(void)
{
    t_shell *shell = malloc(sizeof(t_shell));
    shell->gcl = gc_init(); // Simulating garbage collector setup
    return shell;
}

void free_mock_shell(t_shell *shell)
{
    free(shell->gcl);
    free(shell);
}

START_TEST(test_ms_operator_error_null_input)
{
    t_shell *shell = create_mock_shell();
    int result = ms_operator_error(NULL, shell);
    ck_assert_int_ne(result, 0); // Expect failure (non-zero) for null input
    free_mock_shell(shell);
}
END_TEST

START_TEST(test_ms_operator_error_valid_input)
{
    t_shell *shell = create_mock_shell();
    int result = ms_operator_error("valid input", shell);
    ck_assert_int_eq(result, 0); // Expect success (0) for valid input
    free_mock_shell(shell);
}
END_TEST

Suite *ms_operator_error_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_operator_error");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_operator_error_null_input);
    tcase_add_test(tc_core, test_ms_operator_error_valid_input);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_operator_error_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
