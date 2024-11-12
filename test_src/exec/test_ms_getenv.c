/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_getenv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: assistant <assistant@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:00:00 by assistant         #+#    #+#             */
/*   Updated: 2024/11/12 11:20:23 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/minishell.h"
#include "../../include/exec.h"
#include "../../lib/libgc/include/libgc.h"
#include "../../lib/libft/include/libft.h"
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
    node->var = ft_strdup(var);
    gc_register(node->var, gcl);
    node->value = value ? ft_strdup(value) : NULL;
    if (node->value)
        gc_register(node->value, gcl);
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

/* Test 1: Retrieve existing variable */
START_TEST(test_ms_getenv_existing_variable)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create environment variables */
    add_env_node(&env_data.env, create_env_node("PATH", "/usr/bin"));
    add_env_node(&env_data.env, create_env_node("HOME", "/home/user"));
    add_env_node(&env_data.env, create_env_node("SHELL", "/bin/bash"));

    /* Call the function */
    char *value = ms_getenv("HOME", &env_data);

    /* Verify the value */
    ck_assert_ptr_nonnull(value);
    ck_assert_str_eq(value, "/home/user");
}
END_TEST

/* Test 2: Variable does not exist */
START_TEST(test_ms_getenv_nonexistent_variable)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create environment variables */
    add_env_node(&env_data.env, create_env_node("PATH", "/usr/bin"));

    /* Call the function */
    char *value = ms_getenv("HOME", &env_data);

    /* Verify the value */
    ck_assert_ptr_null(value);
}
END_TEST

/* Test 3: Null name */
START_TEST(test_ms_getenv_null_name)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create environment variables */
    add_env_node(&env_data.env, create_env_node("PATH", "/usr/bin"));

    /* Call the function */
    char *value = ms_getenv(NULL, &env_data);

    /* Verify the value */
    ck_assert_ptr_null(value);
}
END_TEST

/* Test 4: Null env_data */
START_TEST(test_ms_getenv_null_env_data)
{
    /* Call the function */
    char *value = ms_getenv("PATH", NULL);

    /* Verify the value */
    ck_assert_ptr_null(value);
}
END_TEST

/* Test 5: Null env_data->env */
START_TEST(test_ms_getenv_null_env_list)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Call the function */
    char *value = ms_getenv("PATH", &env_data);

    /* Verify the value */
    ck_assert_ptr_null(value);
}
END_TEST

/* Test 6: Empty name string */
START_TEST(test_ms_getenv_empty_name)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create environment variable with empty name */
    add_env_node(&env_data.env, create_env_node("", "empty"));

    /* Call the function */
    char *value = ms_getenv("", &env_data);

    /* Verify the value */
    ck_assert_ptr_nonnull(value);
    ck_assert_str_eq(value, "empty");
}
END_TEST

/* Test 7: Variable with similar names */
START_TEST(test_ms_getenv_similar_names)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create environment variables */
    add_env_node(&env_data.env, create_env_node("PATH", "/usr/bin"));
    add_env_node(&env_data.env, create_env_node("PATHS", "/usr/local/bin"));

    /* Call the function */
    char *value = ms_getenv("PATH", &env_data);

    /* Verify the value */
    ck_assert_ptr_nonnull(value);
    ck_assert_str_eq(value, "/usr/bin");
}
END_TEST

/* Test 8: Case sensitivity */
START_TEST(test_ms_getenv_case_sensitivity)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create environment variables */
    add_env_node(&env_data.env, create_env_node("PATH", "/usr/bin"));
    add_env_node(&env_data.env, create_env_node("path", "/usr/local/bin"));

    /* Call the function */
    char *value_upper = ms_getenv("PATH", &env_data);
    char *value_lower = ms_getenv("path", &env_data);

    /* Verify the values */
    ck_assert_ptr_nonnull(value_upper);
    ck_assert_str_eq(value_upper, "/usr/bin");

    ck_assert_ptr_nonnull(value_lower);
    ck_assert_str_eq(value_lower, "/usr/local/bin");
}
END_TEST

/* Test 9: Variable with NULL value */
START_TEST(test_ms_getenv_null_value)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create environment variable with NULL value */
    add_env_node(&env_data.env, create_env_node("EMPTY", NULL));

    /* Call the function */
    char *value = ms_getenv("EMPTY", &env_data);

    /* Verify the value */
    ck_assert_ptr_null(value);
}
END_TEST

/* Test 10: Variable names with special characters */
START_TEST(test_ms_getenv_special_characters)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create environment variables */
    add_env_node(&env_data.env, create_env_node("VAR$", "dollar"));
    add_env_node(&env_data.env, create_env_node("VAR_SPACE", "space"));

    /* Call the function */
    char *value = ms_getenv("VAR$", &env_data);

    /* Verify the value */
    ck_assert_ptr_nonnull(value);
    ck_assert_str_eq(value, "dollar");
}
END_TEST

/* Test 11: Very long variable name and value */
START_TEST(test_ms_getenv_long_variable)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create long variable name and value */
    char long_name[1024];
    char long_value[1024];
    memset(long_name, 'A', sizeof(long_name) - 1);
    long_name[sizeof(long_name) - 1] = '\0';
    memset(long_value, 'B', sizeof(long_value) - 1);
    long_value[sizeof(long_value) - 1] = '\0';

    add_env_node(&env_data.env, create_env_node(long_name, long_value));

    /* Call the function */
    char *value = ms_getenv(long_name, &env_data);

    /* Verify the value */
    ck_assert_ptr_nonnull(value);
    ck_assert_str_eq(value, long_value);
}
END_TEST

/* Test 12: Duplicate variable names */
START_TEST(test_ms_getenv_duplicate_names)
{
    t_env_data env_data;
    env_data.env = NULL;

    /* Create duplicate environment variables */
    add_env_node(&env_data.env, create_env_node("DUPLICATE", "first"));
    add_env_node(&env_data.env, create_env_node("DUPLICATE", "second"));

    /* Call the function */
    char *value = ms_getenv("DUPLICATE", &env_data);

    /* Verify the value */
    ck_assert_ptr_nonnull(value);
    ck_assert_str_eq(value, "second"); // Since it was added last and is at the head
}
END_TEST

/* Test Suite Setup */
Suite *ms_getenv_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_getenv");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_getenv_existing_variable);
    tcase_add_test(tc_core, test_ms_getenv_nonexistent_variable);
    tcase_add_test(tc_core, test_ms_getenv_null_name);
    tcase_add_test(tc_core, test_ms_getenv_null_env_data);
    tcase_add_test(tc_core, test_ms_getenv_null_env_list);
    tcase_add_test(tc_core, test_ms_getenv_empty_name);
    tcase_add_test(tc_core, test_ms_getenv_similar_names);
    tcase_add_test(tc_core, test_ms_getenv_case_sensitivity);
    tcase_add_test(tc_core, test_ms_getenv_null_value);
    tcase_add_test(tc_core, test_ms_getenv_special_characters);
    tcase_add_test(tc_core, test_ms_getenv_long_variable);
    tcase_add_test(tc_core, test_ms_getenv_duplicate_names);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_getenv_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
