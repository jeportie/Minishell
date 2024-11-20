/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_set_env_value.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:00:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/20 12:48:45 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/minishell.h"

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

// Test pour vérifier si ms_set_env_value ajoute une nouvelle variable
START_TEST(test_ms_set_env_value_new_var)
{
    t_shell shell;
    setup_shell(&shell);
    char *var = gc_strdup("NEW_VAR", shell.gcl);
    gc_register(var, shell.gcl);
    gc_lock(var, shell.gcl);
    char *value = gc_strdup("new_value", shell.gcl);
    gc_register(value, shell.gcl);
    gc_lock(value, shell.gcl);
    ms_set_env_value(&shell, var, value);

    // Vérifier que la variable a bien été ajoutée
    t_env *env = shell.env_data->env;
    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "NEW_VAR");
    ck_assert_str_eq(env->value, "new_value");

    teardown_shell(&shell);
}
END_TEST

// Test pour vérifier si ms_set_env_value met à jour une variable existante
START_TEST(test_ms_set_env_value_update_var)
{
    t_shell shell;
    setup_shell(&shell);
    char *var_ini = gc_strdup("EXISTING_VAR", shell.gcl);
    gc_register(var_ini, shell.gcl);
    gc_lock(var_ini, shell.gcl);
    char *value_ini = gc_strdup("initial_value", shell.gcl);
    gc_register(value_ini, shell.gcl);
    gc_lock(value_ini, shell.gcl);

    // Ajouter une variable initiale
    ms_set_env_value(&shell, var_ini, value_ini);

    char *var_upd = gc_strdup("EXISTING_VAR", shell.gcl);
    gc_register(var_upd, shell.gcl);
    gc_lock(var_upd, shell.gcl);
    char *value_upd = gc_strdup("updated_value", shell.gcl);
    gc_register(value_upd, shell.gcl);
    gc_lock(value_upd, shell.gcl);

    // Mettre à jour cette variable
    ms_set_env_value(&shell, var_upd, value_upd);

    // Vérifier que la variable a bien été mise à jour
    t_env *env = shell.env_data->env;
    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "EXISTING_VAR");
    ck_assert_str_eq(env->value, "updated_value");

    teardown_shell(&shell);
}
END_TEST

// Suite de tests pour ms_set_env_value
static Suite *ms_set_env_value_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_set_env_value");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_set_env_value_new_var);
    tcase_add_test(tc_core, test_ms_set_env_value_update_var);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_set_env_value_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

