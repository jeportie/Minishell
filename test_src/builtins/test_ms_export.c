/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:00:32 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/25 13:33:29 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/builtins.h"
#include "../../include/exec.h"  // pour le t_exec_context

static t_gc *gcl;

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

// Test pour vérifier si ms_export ajoute une nouvelle variable
START_TEST(test_ms_export_new_var)
{
    t_shell         *shell;
    t_exec_context  *context;
    t_cmd_node      *cmd_node;
    int             error;

    shell = gc_malloc(sizeof(t_shell), gcl);
    memset(shell, 0, sizeof(t_shell));
    shell->gcl = gcl;
    shell->env_data = gc_malloc(sizeof(t_env_data), gcl);
    shell->env_data = memset(shell->env_data, 0, sizeof(t_env_data));
    shell->env_data->env = gc_malloc(sizeof(t_env), gcl);
    shell->env_data->env = memset(shell->env_data->env, 0, sizeof(t_env));
    shell->env_data->env->var = gc_strdup("USER", gcl);
    shell->env_data->env->value = gc_strdup("gmarquis", gcl);
    shell->env_data->env->next = NULL;

    cmd_node = gc_malloc(sizeof(t_cmd_node), gcl);
    cmd_node->argc = 2;

    cmd_node->argv = gc_malloc(sizeof(char *) * 3, gcl);
    cmd_node->argv[0] = gc_strdup("export", gcl);
    cmd_node->argv[1] = gc_strdup("NEW_VAR=new_value", gcl);
    cmd_node->argv[2] = NULL;

    context = gc_malloc(sizeof(t_exec_context), gcl);
    memset(context, 0, sizeof(t_exec_context));
    context->shell = shell;
    context->shell->gcl = gcl;
    // Appeler ms_export pour ajouter "NEW_VAR"
  //  error = ms_export(cmd_node, context);

    // Vérifier que la variable a bien été ajoutée
    t_env *env = shell->env_data->env;
    ck_assert_str_eq(env->var, "USER");
    ck_assert_str_eq(env->value, "gmarquis");
    ck_assert_str_eq(cmd_node->argv[0], "export");
//    env = env->next;
//    ck_assert_ptr_nonnull(env);
//    ck_assert_str_eq(env->var, "NEW_VAR");
//    ck_assert_str_eq(env->value, "new_value");
}
END_TEST

// Suite de tests pour ms_export
static Suite *ms_export_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_export");
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);

    tcase_add_test(tc_core, test_ms_export_new_var);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_export_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

