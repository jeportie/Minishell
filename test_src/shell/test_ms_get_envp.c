/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_get_envp.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:37:09 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 15:26:32 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/minishell.h"
#include "../../include/exec.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static t_gc *gcl;

/* Helper function to create an environment variable node */
t_env *create_env_node(const char *var, const char *value)
{
    t_env *node = gc_malloc(sizeof(t_env), gcl);
    if (!node)
    {
        perror("gc_malloc");
        exit(EXIT_FAILURE);
    }
    node->var = (char *)var;   // Assuming var is managed elsewhere or is a literal
    node->value = (char *)value; // Same for value
    node->next = NULL;
    return node;
}

/* Helper function to add a node to the environment list */
void add_env_node(t_env **head, t_env *new_node)
{
    new_node->next = *head;
    *head = new_node;
}

/* Setup and Teardown functions */
void setup(void)
{
    gcl = gc_init();
    if (!gcl)
    {
        perror("gc_init");
        exit(EXIT_FAILURE);
    }
}

void teardown(void)
{
    gc_cleanup(gcl);
}

/* Test Cases */

/* Test 1: Convert a typical environment list */
START_TEST(test_ms_get_envp_typical)
{
    t_env *env = NULL;

    /* Create environment variables */
    add_env_node(&env, create_env_node("PATH", "/usr/bin"));
    add_env_node(&env, create_env_node("HOME", "/home/user"));
    add_env_node(&env, create_env_node("SHELL", "/bin/bash"));

    /* Call the function */
    char **envp = ms_get_envp(env, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);

    /* Since we added nodes to the head, the order is reversed */
    ck_assert_str_eq(envp[0], "SHELL=/bin/bash");
    ck_assert_str_eq(envp[1], "HOME=/home/user");
    ck_assert_str_eq(envp[2], "PATH=/usr/bin");
    ck_assert_ptr_null(envp[3]);
}
END_TEST

/* Test 2: Empty environment list */
START_TEST(test_ms_get_envp_empty_list)
{
    t_env *env = NULL;

    /* Call the function */
    char **envp = ms_get_envp(env, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);
    ck_assert_ptr_null(envp[0]);
}
END_TEST

/* Test 3: Null environment pointer */
START_TEST(test_ms_get_envp_null_env)
{
    /* Call the function with NULL env */
    char **envp = ms_get_envp(NULL, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);
    ck_assert_ptr_null(envp[0]);
}
END_TEST


/* Test 5: Variable with NULL var */
START_TEST(test_ms_get_envp_null_var)
{
    t_env *env = NULL;
    add_env_node(&env, create_env_node(NULL, "value"));

    /* Call the function */
    char **envp = ms_get_envp(env, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);
    ck_assert_ptr_nonnull(envp[0]);
    ck_assert_ptr_null(envp[1]);

    ck_assert_str_eq(envp[0], "=value");
}
END_TEST

/* Test 6: Variable with NULL value */
START_TEST(test_ms_get_envp_null_value)
{
    t_env *env = NULL;
    add_env_node(&env, create_env_node("VAR", NULL));

    /* Call the function */
    char **envp = ms_get_envp(env, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);
    ck_assert_ptr_nonnull(envp[0]);
    ck_assert_ptr_null(envp[1]);

    ck_assert_str_eq(envp[0], "VAR=");
}
END_TEST

/* Test 7: Variable with empty var and value */
START_TEST(test_ms_get_envp_empty_var_value)
{
    t_env *env = NULL;
    add_env_node(&env, create_env_node("", ""));

    /* Call the function */
    char **envp = ms_get_envp(env, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);
    ck_assert_ptr_nonnull(envp[0]);
    ck_assert_ptr_null(envp[1]);

    ck_assert_str_eq(envp[0], "=");
}
END_TEST

/* Test 8: Variables with special characters */
START_TEST(test_ms_get_envp_special_characters)
{
    t_env *env = NULL;
    add_env_node(&env, create_env_node("VAR$", "value$"));
    add_env_node(&env, create_env_node("VAR=NAME", "value=equals"));

    /* Call the function */
    char **envp = ms_get_envp(env, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);
    ck_assert_ptr_nonnull(envp[0]);
    ck_assert_ptr_nonnull(envp[1]);
    ck_assert_ptr_null(envp[2]);

    ck_assert_str_eq(envp[0], "VAR=NAME=value=equals");
    ck_assert_str_eq(envp[1], "VAR$=value$");
}
END_TEST

/* Test 9: Variables with long names and values */
START_TEST(test_ms_get_envp_long_var_value)
{
    t_env *env = NULL;

    /* Create long variable name and value */
    char long_var[1024];
    char long_value[1024];
    memset(long_var, 'A', sizeof(long_var) - 1);
    long_var[sizeof(long_var) - 1] = '\0';
    memset(long_value, 'B', sizeof(long_value) - 1);
    long_value[sizeof(long_value) - 1] = '\0';

    add_env_node(&env, create_env_node(long_var, long_value));

    /* Call the function */
    char **envp = ms_get_envp(env, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);
    ck_assert_ptr_nonnull(envp[0]);
    ck_assert_ptr_null(envp[1]);

    /* Verify the content */
    size_t expected_len = strlen(long_var) + 1 + strlen(long_value);
    ck_assert_int_eq(strlen(envp[0]), expected_len);

    /* Optionally, compare the entire string if necessary */
}
END_TEST

/* Test 10: Duplicate variable names */
START_TEST(test_ms_get_envp_duplicate_names)
{
    t_env *env = NULL;

    add_env_node(&env, create_env_node("DUPLICATE", "first"));
    add_env_node(&env, create_env_node("DUPLICATE", "second"));

    /* Call the function */
    char **envp = ms_get_envp(env, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);
    ck_assert_ptr_nonnull(envp[0]);
    ck_assert_ptr_nonnull(envp[1]);
    ck_assert_ptr_null(envp[2]);

    ck_assert_str_eq(envp[0], "DUPLICATE=second");
    ck_assert_str_eq(envp[1], "DUPLICATE=first");
}
END_TEST

/* Test 11: Ensure original environment list is not adversely modified */
START_TEST(test_ms_get_envp_env_list_integrity)
{
    t_env *env = NULL;
    t_env *node = create_env_node("VAR", "value");
    add_env_node(&env, node);

    /* Call the function */
    char **envp = ms_get_envp(env, gcl);

    /* Verify the result */
    ck_assert_ptr_nonnull(envp);
    ck_assert_ptr_nonnull(envp[0]);
    ck_assert_ptr_null(envp[1]);

    ck_assert_str_eq(envp[0], "VAR=value");

    /* Ensure env->var and env->value are unchanged */
    ck_assert_str_eq(node->var, "VAR");
    ck_assert_str_eq(node->value, "value");
}
END_TEST

/* Test Suite Setup */
Suite *ms_get_envp_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_get_envp");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_get_envp_typical);
    tcase_add_test(tc_core, test_ms_get_envp_empty_list);
    tcase_add_test(tc_core, test_ms_get_envp_null_env);
    tcase_add_test(tc_core, test_ms_get_envp_null_var);
    tcase_add_test(tc_core, test_ms_get_envp_null_value);
    tcase_add_test(tc_core, test_ms_get_envp_empty_var_value);
    tcase_add_test(tc_core, test_ms_get_envp_special_characters);
    tcase_add_test(tc_core, test_ms_get_envp_long_var_value);
    tcase_add_test(tc_core, test_ms_get_envp_duplicate_names);
    tcase_add_test(tc_core, test_ms_get_envp_env_list_integrity);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_get_envp_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
