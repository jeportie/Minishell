/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_set_env_value.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:46:30 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 18:07:15 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <string.h>
#include "../../include/env_value.h"

// Fonction de création d'un nœud pour la liste d'environnement (utile pour le test)
t_env *create_env_node(t_shell *shell, char *var, char *value)
{
    t_env *node = gc_malloc(sizeof(t_env), shell->gcl);
    gc_lock(node, shell->gcl);
    if (!node)
        return NULL;
    node->var = strdup(var);
    gc_register(node->var, shell->gcl);
    gc_lock(node->var, shell->gcl);
    node->value = strdup(value);
    gc_register(node->value, shell->gcl);
    gc_lock(node->value, shell->gcl);
    node->next = NULL;
    return node;
}

// Test pour ajouter une nouvelle variable d'environnement
START_TEST(test_ms_set_env_value_add_new)
{
    t_shell shell;
    shell.gcl = gc_init();
    shell.env_data = gc_malloc(sizeof(t_env_data), shell.gcl);
    gc_lock(shell.env_data, shell.gcl);
    shell.env_data->env = create_env_node(&shell, "USER", "gmarquis");
    shell.env_data->env->next = create_env_node(&shell, "PATH", "/usr/bin");
    shell.env_data->env->next->next = create_env_node(&shell, "SHELL", "/bin/bash");

    ms_set_env_value(&shell, "NEW_VAR", "new_value");

    // Parcours de la liste pour vérifier que la nouvelle variable a été ajoutée
    t_env *env = shell.env_data->env;
    while (env->next != NULL) {
        env = env->next;
    }
    
    // Vérifie que le dernier nœud correspond à la nouvelle variable
    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "NEW_VAR");
    ck_assert_str_eq(env->value, "new_value");

    // Nettoyage
    gc_cleanup(shell.gcl);
    free(shell.gcl);
}
END_TEST

// Test pour mettre à jour une variable d'environnement existante
START_TEST(test_ms_set_env_value_update_existing)
{
    t_shell shell;
    shell.gcl = gc_init();
    shell.env_data = gc_malloc(sizeof(t_env_data), shell.gcl);
    gc_lock(shell.env_data, shell.gcl);
    shell.env_data->env = create_env_node(&shell, "USER", "gmarquis");
    shell.env_data->env->next = create_env_node(&shell, "PATH", "/usr/bin");
    shell.env_data->env->next->next = create_env_node(&shell, "SHELL", "/bin/bash");

    ms_set_env_value(&shell, "USER", "jeportie");

    // Vérifie que la variable existante a été mise à jour
    t_env *env = shell.env_data->env;
    ck_assert_ptr_nonnull(env);
    ck_assert_str_eq(env->var, "USER");
    ck_assert_str_eq(env->value, "jeportie");

    // Nettoyage
    gc_cleanup(shell.gcl);
    free(shell.gcl);
}
END_TEST

// Suite de tests pour ms_set_env_value
static Suite *ms_set_env_value_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_set_env_value");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_set_env_value_add_new);
    tcase_add_test(tc_core, test_ms_set_env_value_update_existing);

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

