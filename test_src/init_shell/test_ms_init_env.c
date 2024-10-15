/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_init_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:38:15 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/15 11:28:11 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <check.h>

START_TEST(test_ms_init_env_with_envp)
{
    t_gc *gcl = gc_init();
    t_shell shell = {.gcl = gcl};
    t_env_data env_data;
    shell.env_data = &env_data;
    char *envp[] = {"PATH=/usr/bin", "HOME=/home/user", "TERM=xterm-256color", "SHELL=/bin/bash", NULL};

    shell.env_data = ms_init_env(envp, &shell);

    ck_assert(shell.env_data->env_exist);
    ck_assert_ptr_nonnull(shell.env_data->env);
    ck_assert_str_eq(shell.env_data->path, "/usr/bin");
    ck_assert_str_eq(shell.env_data->term, "xterm-256color");
    ck_assert_str_eq(shell.env_data->shell, "/bin/bash");

    // Check that environment variables are properly set
    t_env *current = shell.env_data->env;
    ck_assert_str_eq(current->var, "PATH");
    ck_assert_str_eq(current->value, "/usr/bin");
    current = current->next;
    ck_assert_str_eq(current->var, "HOME");
    ck_assert_str_eq(current->value, "/home/user");
    current = current->next;
    ck_assert_str_eq(current->var, "TERM");
    ck_assert_str_eq(current->value, "xterm-256color");
    current = current->next;
    ck_assert_str_eq(current->var, "SHELL");
    ck_assert_str_eq(current->value, "/bin/bash");
    ck_assert_ptr_null(current->next);

    gc_cleanup(gcl);
    free(gcl);
}
END_TEST

START_TEST(test_ms_init_env_without_envp)
{
    t_gc *gcl = gc_init();
    t_shell shell = {.gcl = gcl};
    t_env_data env_data;
    shell.env_data = &env_data;

    shell.env_data = ms_init_env(NULL, &shell);

    ck_assert(!shell.env_data->env_exist);
    ck_assert_ptr_nonnull(shell.env_data->env);
    ck_assert_str_eq(shell.env_data->path, "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
    ck_assert_str_eq(shell.env_data->term, "dumb");
    ck_assert_str_eq(shell.env_data->shell, "/bin/zsh");

    // Check that default environment variables are properly set
    t_env *current = shell.env_data->env;
    ck_assert_str_eq(current->var, "SHLVL");
    ck_assert_str_eq(current->value, "1");
    current = current->next;
    ck_assert_str_eq(current->var, "PWD");
    ck_assert_ptr_nonnull(current->value);
    ck_assert_ptr_null(current->next);

    gc_cleanup(gcl);
    free(gcl);
}
END_TEST

Suite *ms_init_env_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_init_env");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_init_env_with_envp);
    tcase_add_test(tc_core, test_ms_init_env_without_envp);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_init_env_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
