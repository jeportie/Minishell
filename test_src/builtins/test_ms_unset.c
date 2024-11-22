/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:00:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/22 17:02:25 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include "../../include/builtins.h"

static	t_gc *gcl;

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
	free(gcl);
}

START_TEST(test_ms_unset_first_value)
{
	t_cmd_node		*cmd_node;
	t_exec_context	*context;
    t_env           *env1 = gc_malloc(sizeof(t_env), gcl);
    t_env           *env2 = gc_malloc(sizeof(t_env), gcl);
    t_env           *env3 = gc_malloc(sizeof(t_env), gcl);

    env1->var = "a";
    env2->var = "b";
    env3->var = "c";
    env1->value = "jerome";
    env2->value = "portier";
    env3->value = "1994";
    env1->next = env2;
    env2->next = env3;
    env3->next = NULL;

    cmd_node = gc_malloc(sizeof(t_cmd_node), gcl);
    cmd_node->argc = 2;
    cmd_node->argv = gc_malloc(sizeof(char) * 3, gcl);
    cmd_node->argv[0] = "unset";
    cmd_node->argv[1] = "a";
    cmd_node->argv[2] = NULL;;
    cmd_node->is_expand = false;
    context = gc_malloc(sizeof(t_exec_context), gcl);
    context->shell = gc_malloc(sizeof(t_shell), gcl);
    context->shell->gcl = gcl;
    context->shell->env_data = gc_malloc(sizeof(t_env_data), gcl);
    memset(context->shell->env_data, 0, sizeof(t_env_data));
    context->shell->env_data->env = env1;
    
	ms_unset(cmd_node, context);
	ck_assert_str_eq(context->shell->env_data->env->var, "b");
//	ck_assert_str_eq(context->shell->env_data->env->value, "portier");
 //   ck_assert_ptr_null(context->shell->env_data->env->next->next);
}
END_TEST

Suite *name_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("NAME");

    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_ms_unset_first_value);
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

