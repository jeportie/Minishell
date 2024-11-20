/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:00:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/20 13:01:43 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/builtins.h"

// Fonction de configuration du shell et du garbage collector pour les tests
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

// Test pour vérifier si ms_unset supprime une variable existante
START_TEST(test_ms_unset_existing_var)
{
    t_shell shell;
    setup_shell(&shell);

    // Ajouter une variable
    char *var = gc_strdup("USER", shell.gcl);
    gc_register(var, shell.gcl);
    gc_lock(var, shell.gcl);
    char *value = gc_strdup("gmarquis", shell.gcl);
    gc_register(value, shell.gcl);
    gc_lock(value, shell.gcl);
    ms_set_env_value(&shell, var, value);

    // Vérifier que la variable USER est présente
    t_env *env = shell.env_data->env;
    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "USER");
    ck_assert_str_eq(env->value, "gmarquis");

    // Préparer les arguments pour ms_unset
    t_cmd_node cmd_node;
    cmd_node.argc = 2;
    char *argv[] = {"unset", "USER", NULL};
    cmd_node.argv = argv;

    // Appeler ms_unset pour supprimer "USER"
    ms_unset(&cmd_node, &(t_exec_context){ .shell = &shell });

    // Vérifier que la variable USER est supprimée
    ck_assert_ptr_null(ms_get_env_value(shell.env_data->env, "USER"));

    teardown_shell(&shell);
}
END_TEST

// Test pour vérifier que ms_unset ne fait rien si la variable n'existe pas
START_TEST(test_ms_unset_non_existing_var)
{
    t_shell shell;
    setup_shell(&shell);

    // Ajouter des variables initiales
    char *var = gc_strdup("PATH", shell.gcl);
    gc_register(var, shell.gcl);
    gc_lock(var, shell.gcl);
    char *value = gc_strdup("/usr/bin", shell.gcl);
    gc_register(value, shell.gcl);
    gc_lock(value, shell.gcl);
    ms_set_env_value(&shell, var, value);

    // Préparer les arguments pour ms_unset
    t_cmd_node cmd_node;
    cmd_node.argc = 2;
    char *argv[] = {"unset", "NON_EXISTING_VAR", NULL};
    cmd_node.argv = argv;

    // Appeler ms_unset pour une variable inexistante
    ms_unset(&cmd_node, &(t_exec_context){ .shell = &shell });

    // Vérifier que les autres variables n'ont pas été modifiées
    ck_assert_str_eq(ms_get_env_value(shell.env_data->env, "PATH"), "/usr/bin");

    teardown_shell(&shell);
}
END_TEST

// Suite de tests pour ms_unset
static Suite *ms_unset_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_unset");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_unset_existing_var);
    tcase_add_test(tc_core, test_ms_unset_non_existing_var);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_unset_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

