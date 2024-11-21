/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:00:32 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 17:54:31 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/builtins.h"
#include "../../include/exec.h"  // pour le t_exec_context

void setup_shell(t_shell *shell) {
    shell->gcl = gc_init();
    shell->env_data = gc_malloc(sizeof(t_env_data), shell->gcl);
    shell->env_data->env = NULL;
}

// Fonction de nettoyage de l'environnement après chaque test
void teardown_shell(t_shell *shell) {
    gc_cleanup(shell->gcl);
    free(shell->gcl);
}

// Test pour vérifier si ms_export ajoute une nouvelle variable
START_TEST(test_ms_export_new_var)
{
    t_shell shell;
    setup_shell(&shell);

    // Ajouter une variable
    ms_set_env_value(&shell, "USER", "gmarquis");

    t_cmd_node cmd_node;
    cmd_node.argc = 2;
    char *argv[] = {"export", "NEW_VAR=new_value", NULL};
    cmd_node.argv = argv;

    // Appeler ms_export pour ajouter "NEW_VAR"
    ms_export(&cmd_node, &(t_exec_context){ .shell = &shell });

    // Vérifier que la variable a bien été ajoutée
    t_env *env = shell.env_data->env;
    ck_assert_str_eq(env->var, "USER");
    ck_assert_str_eq(env->value, "gmarquis");
    env = env->next;
    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "NEW_VAR");
    ck_assert_str_eq(env->value, "new_value");

    teardown_shell(&shell);
}
END_TEST

// Test pour vérifier si ms_export met à jour une variable existante
START_TEST(test_ms_export_update_var)
{
    t_shell shell;
    setup_shell(&shell);

    // Ajouter une variable
    ms_set_env_value(&shell, "USER", "gmarquis");
    ms_set_env_value(&shell, "EXISTING_VAR", "initial_value");

    // Ajouter une variable initiale
    t_cmd_node cmd_node;
    cmd_node.argc = 2;
    char *argv[] = {"export", "EXISTING_VAR=updated_value", NULL};
    cmd_node.argv = argv;

    ms_export(&cmd_node, &(t_exec_context){ .shell = &shell });

    // Vérifier que la variable a bien été mise à jour
    t_env *env = shell.env_data->env->next;
    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "EXISTING_VAR");
    ck_assert_str_eq(env->value, "updated_value");

    teardown_shell(&shell);
}
END_TEST

// Suite de tests pour ms_export
static Suite *ms_export_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_export");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_export_new_var);
    tcase_add_test(tc_core, test_ms_export_update_var);

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

