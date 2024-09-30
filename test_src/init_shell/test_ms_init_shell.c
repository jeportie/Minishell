/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_init_shell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:43:18 by jeportie          #+#    #+#             */
/*   Updated: 2024/09/30 16:13:17 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include "../../include/minishell.h"

START_TEST(test_ms_init_shell_basic)
{
    t_shell shell = ms_init_shell(0, NULL, NULL);
    ck_assert_ptr_null(shell.user_input);
    ck_assert_int_eq(shell.error_code, 0);
    ck_assert_ptr_nonnull(shell.gcl);
}
END_TEST

START_TEST(test_ms_init_shell_with_envp)
{
    char *envp[] = {"PATH=/usr/bin", "HOME=/home/user", NULL};
    t_shell shell = ms_init_shell(2, NULL, envp);
    ck_assert_ptr_null(shell.user_input);
    ck_assert_int_eq(shell.error_code, 0);
    ck_assert_ptr_nonnull(shell.gcl);
}
END_TEST

START_TEST(test_ms_init_shell_with_argc)
{
    t_shell shell = ms_init_shell(5, NULL, NULL);
    ck_assert_ptr_null(shell.user_input);
    ck_assert_int_eq(shell.error_code, 0);
    ck_assert_ptr_nonnull(shell.gcl);
}
END_TEST

START_TEST(test_ms_init_shell_with_large_argc)
{
    t_shell shell = ms_init_shell(1000, NULL, NULL);
    ck_assert_ptr_null(shell.user_input);
    ck_assert_int_eq(shell.error_code, 0);
    ck_assert_ptr_nonnull(shell.gcl);
}
END_TEST

START_TEST(test_ms_init_shell_with_empty_envp)
{
    char *envp[] = {NULL};
    t_shell shell = ms_init_shell(1, NULL, envp);
    ck_assert_ptr_null(shell.user_input);
    ck_assert_int_eq(shell.error_code, 0);
    ck_assert_ptr_nonnull(shell.gcl);
}
END_TEST

START_TEST(test_ms_init_shell_with_null_envp)
{
    t_shell shell = ms_init_shell(1, NULL, NULL);
    ck_assert_ptr_null(shell.user_input);
    ck_assert_int_eq(shell.error_code, 0);
    ck_assert_ptr_nonnull(shell.gcl);
}
END_TEST

START_TEST(test_ms_init_shell_with_special_chars_in_envp)
{
    char *envp[] = {"PATH=/usr/bin", "SPECIAL=*&^%$#@!", NULL};
    t_shell shell = ms_init_shell(2, NULL, envp);
    ck_assert_ptr_null(shell.user_input);
    ck_assert_int_eq(shell.error_code, 0);
    ck_assert_ptr_nonnull(shell.gcl);
}
END_TEST

START_TEST(test_ms_init_shell_with_long_envp)
{
    char *envp[] = {"PATH=/usr/bin", "LONG_ENV=abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", NULL};
    t_shell shell = ms_init_shell(2, NULL, envp);
    ck_assert_ptr_null(shell.user_input);
    ck_assert_int_eq(shell.error_code, 0);
    ck_assert_ptr_nonnull(shell.gcl);
}
END_TEST

START_TEST(test_ms_init_shell_with_multiple_envp)
{
    char *envp[] = {"PATH=/usr/bin", "HOME=/home/user", "SHELL=/bin/bash", "USER=test", NULL};
    t_shell shell = ms_init_shell(4, NULL, envp);
    ck_assert_ptr_null(shell.user_input);
    ck_assert_int_eq(shell.error_code, 0);
    ck_assert_ptr_nonnull(shell.gcl);
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

