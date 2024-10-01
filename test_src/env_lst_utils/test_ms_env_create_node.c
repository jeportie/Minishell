/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_env_create_node.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:28:21 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/01 16:28:51 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/minishell.h"

// Mock functions and structures
void *gc_malloc(size_t size, t_gc gc) {
    return malloc(size);
}

// Test cases
START_TEST(test_ms_env_create_node_returns_null) {
    t_shell shell;
    t_gc gc;
    shell.gcl = &gc;

    t_env *node = ms_env_create_node(&shell);

    ck_assert_ptr_eq(node, NULL);
}
END_TEST

Suite *minishell_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Minishell");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_env_create_node_returns_null);
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

