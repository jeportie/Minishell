/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_env_add_back.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:02:06 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/01 15:02:28 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/minishell.h"

// Helper function to create a new environment node
t_env *create_env_node(const char *var, const char *value) {
    t_env *new_node = (t_env *)malloc(sizeof(t_env));
    if (!new_node)
        return NULL;
    new_node->var = strdup(var);
    new_node->value = strdup(value);
    new_node->next = NULL;
    return new_node;
}

// Helper function to free the environment list
void free_env_list(t_env *env) {
    t_env *tmp;
    while (env) {
        tmp = env;
        env = env->next;
        free(tmp->var);
        free(tmp->value);
        free(tmp);
    }
}

// Test adding a node to an empty list
START_TEST(test_add_to_empty_list) {
    t_env *env = NULL;
    t_env *new_node = create_env_node("USER", "root");

    ms_env_add_back(&env, new_node);

    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "USER");
    ck_assert_str_eq(env->value, "root");
    ck_assert_ptr_null(env->next);

    free_env_list(env);
}
END_TEST

// Test adding a node to a non-empty list
START_TEST(test_add_to_non_empty_list) {
    t_env *env = create_env_node("USER", "root");
    t_env *new_node = create_env_node("HOME", "/root");

    ms_env_add_back(&env, new_node);

    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "USER");
    ck_assert_str_eq(env->value, "root");
    ck_assert_ptr_nonnull(env->next);
    ck_assert_str_eq(env->next->var, "HOME");
    ck_assert_str_eq(env->next->value, "/root");
    ck_assert_ptr_null(env->next->next);

    free_env_list(env);
}
END_TEST

// Test adding a node with NULL env pointer
START_TEST(test_add_with_null_env) {
    t_env *new_node = create_env_node("USER", "root");

    ms_env_add_back(NULL, new_node);

    // Since env is NULL, new_node should not be added anywhere
    ck_assert_ptr_nonnull(new_node);
    ck_assert_str_eq(new_node->var, "USER");
    ck_assert_str_eq(new_node->value, "root");
    ck_assert_ptr_null(new_node->next);

    free_env_list(new_node);
}
END_TEST

// Test adding a NULL node
START_TEST(test_add_null_node) {
    t_env *env = create_env_node("USER", "root");

    ms_env_add_back(&env, NULL);

    // Since new_node is NULL, env should remain unchanged
    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "USER");
    ck_assert_str_eq(env->value, "root");
    ck_assert_ptr_null(env->next);

    free_env_list(env);
}
END_TEST

Suite *minishell_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Minishell");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_add_to_empty_list);
    tcase_add_test(tc_core, test_add_to_non_empty_list);
    tcase_add_test(tc_core, test_add_with_null_env);
    tcase_add_test(tc_core, test_add_null_node);
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

