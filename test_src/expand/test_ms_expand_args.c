/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_expand_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:24:48 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 14:08:47 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../lib/libgc/include/libgc.h"
#include "../../include/minishell.h"
#include "../../include/expand.h"

static	t_gc *gcl;

void	setup(void)
{
	gcl = gc_init();
	if (!gcl)
	{
		perror("gc_init");
		exit(EXIT_FAILURE);
	}
}

void	teardown(void)
{
	gc_cleanup(gcl);
}

START_TEST(test_ms_expand_arg_simple_expand)
{
	char	*result;
	t_shell	*shell;
	char	*arg;
	bool	is_nested;

	shell = gc_malloc(sizeof(t_shell), gcl);
	memset(shell, 0, sizeof(t_shell));
	shell->gcl = gcl;
	shell->env_data = gc_malloc(sizeof(t_env_data), gcl);
	memset(shell->env_data, 0, sizeof(t_env_data));
	shell->env_data->env = gc_malloc(sizeof(t_env), gcl);
	memset(shell->env_data->env, 0, sizeof(t_env));
	shell->env_data->env->var = gc_strdup("a", gcl);
	shell->env_data->env->value = gc_strdup("test", gcl);
	shell->env_data->env->next = NULL;
	arg = "$a";
	is_nested = false;

	result = ms_expand_arg(arg, shell, is_nested);
	ck_assert_str_eq(result, "test");
}
END_TEST

Suite *name_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("NAME");

    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_ms_expand_arg_simple_expand);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *name;
    SRunner *name_r;

    name = name_suite();
    name_r = srunner_create(name);

    srunner_run_all(name_r, CK_NORMAL); 
    number_failed = srunner_ntests_failed(name_r);
    srunner_free(name_r);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*
## ck_assert functions in the Check framework:
|-------------------------------|---------------------------------------------|
| **Function**                  | **Description -> Check if ...**             |
|-------------------------------|---------------------------------------------|
|ck_assert(expr)                |the expression expr is true (non-zero).      |
|ck_assert_msg(expr, ...)       |expr is true, with message on failure.       |
|ck_assert_int_eq(val1, val2)   |two integers are equal.                      |
|ck_assert_int_ne(val1, val2)   |two integers are not equal.                  |
|ck_assert_int_lt(val1, val2)   |val1 is less than  al2.                      |
|ck_assert_int_le(val1, val2)   |val1 is less than or equal to `val2`.        |
|ck_assert_int_gt(val1, val2)   |val1 is greater than `val2`.                 |
|ck_assert_int_ge(val1, val2)   |val1 is greater than or equal to `val2`.     |
|ck_assert_float_eq(val1, val2) |two floats are equal.                        |
|ck_assert_float_ne(val1, val2) |two floats are not equal.                    |
|ck_assert_float_lt(val1, val2) |val1 is less than `val2`.                    |
|ck_assert_float_le(val1, val2) |val1 is less than or equal to `val2`.        |
|ck_assert_float_gt(val1, val2) |val1 is greater than val2.                   |
|ck_assert_float_ge(val1, val2) |val1 is greater than or equal to val2.       |
|ck_assert_ptr_eq(ptr1, ptr2)   |two pointers are equal.                      |
|ck_assert_ptr_ne(ptr1, ptr2)   |two pointers are not equal.                  |
|ck_assert_str_eq(str1, str2)   |two strings are equal.                       |
|ck_assert_str_ne(str1, str2)   |two strings are not equal.                   |
|ck_assert_str_lt(str1, str2)   |str1 is less than str2.                      |
|ck_assert_str_le(str1, str2)   |str1 is less than or equal to str2.          |
|ck_assert_str_gt(str1, str2)   |str1 is greater than str2.                   |
|ck_assert_str_ge(str1, str2)   |str1 is greater than or equal to str2.       |
|ck_assert_mem_eq(ptr1, ptr2, n)|two memory regions are equal for n bytes.    |
|ck_assert_mem_ne(ptr1, ptr2, n)|two memory regions are not equal for n bytes.|
|-------------------------------|---------------------------------------------|
*/
