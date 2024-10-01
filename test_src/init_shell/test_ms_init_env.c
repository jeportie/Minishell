/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_init_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:38:15 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/01 16:32:25 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <check.h>

// Test cases
START_TEST(test_ms_init_env_with_envp) {
    char *envp[] = {"PATH=/usr/bin", "HOME=/home/user", NULL};
    t_shell shell;
    t_gc gc;
    shell.gcl = &gc;

    t_env_data env_data = ms_init_env(envp, &shell);

    ck_assert(env_data.env_exist == true);
    ck_assert(env_data.env == NULL);
    ck_assert(env_data.path == NULL);
}
END_TEST

START_TEST(test_ms_init_env_without_envp) {
    char **envp = NULL;
    t_shell shell;
    t_gc gc;
    shell.gcl = &gc;

    t_env_data env_data = ms_init_env(envp, &shell);

    ck_assert(env_data.env_exist == true);
    ck_assert(env_data.env != NULL);
    ck_assert_str_eq(env_data.env->var, "MOCK_VAR");
    ck_assert_str_eq(env_data.env->value, "MOCK_VALUE");
    ck_assert(env_data.env->next == NULL);
    ck_assert(env_data.path == NULL);
}
END_TEST

Suite *minishell_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Minishell");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_init_env_with_envp);
    tcase_add_test(tc_core, test_ms_init_env_without_envp);
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

