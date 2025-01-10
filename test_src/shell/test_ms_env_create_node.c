/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_env_create_node.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:28:21 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 14:09:12 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/minishell.h"

START_TEST(test_create_node_basic)
{
    t_shell shell;
    shell.gcl = gc_init(); // Initialize your garbage collector

    const char *env_line = "PATH=/usr/bin";
    t_env *node = ms_env_create_node(&shell, env_line);

    ck_assert_msg(strcmp(node->var, "PATH") == 0, "Expected var: PATH, but got: %s", node->var);
    ck_assert_msg(strcmp(node->value, "/usr/bin") == 0, "Expected value: /usr/bin, but got: %s", node->value);

    gc_cleanup(shell.gcl); // Clean up your garbage collector
}
END_TEST

START_TEST(test_create_node_no_value)
{
    t_shell shell;
    shell.gcl = gc_init(); // Initialize your garbage collector

    const char *env_line = "EMPTY=";
    t_env *node = ms_env_create_node(&shell, env_line);

    ck_assert_msg(strcmp(node->var, "EMPTY") == 0, "Expected var: EMPTY, but got: %s", node->var);
    ck_assert_msg(strcmp(node->value, "") == 0, "Expected value: (empty string), but got: %s", node->value);

    gc_cleanup(shell.gcl); // Clean up your garbage collector
}
END_TEST

START_TEST(test_create_node_no_equals)
{
    t_shell shell;
    shell.gcl = gc_init(); // Initialize your garbage collector

    const char *env_line = "NOEQUALS";
    t_env *node = ms_env_create_node(&shell, env_line);

    ck_assert_msg(strcmp(node->var, "NOEQUALS") == 0, "Expected var: NOEQUALS, but got: %s", node->var);
    ck_assert_msg(node->value == NULL, "Expected value: NULL, but got: %s", node->value ? node->value : "(null)");

    gc_cleanup(shell.gcl);
}
END_TEST

Suite *env_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Env");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_create_node_basic);
    tcase_add_test(tc_core, test_create_node_no_value);
    tcase_add_test(tc_core, test_create_node_no_equals);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = env_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

