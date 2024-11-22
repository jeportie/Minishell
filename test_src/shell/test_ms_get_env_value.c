/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_get_env_value.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:29:33 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/22 09:49:20 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/minishell.h"

// Fonction de création d'un nœud pour la liste d'environnement (utile pour le test)
t_env *create_env_node(char *var, char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return NULL;
    node->var = strdup(var);
    node->value = strdup(value);
    node->next = NULL;
    return node;
}

// Libération de la liste des variables d'environnement après chaque test
void free_env_list(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env->next;
        free(env->var);
        free(env->value);
        free(env);
        env = tmp;
    }
}

// Test pour vérifier si ms_get_env_value retourne la bonne valeur
START_TEST(test_ms_get_env_value_existing)
{
    // Création de la liste des variables d'environnement
    t_env *env_list = create_env_node("USER", "gmarquis");
    env_list->next = create_env_node("PATH", "/usr/bin");
    env_list->next->next = create_env_node("SHELL", "/bin/bash");

    // Vérification des valeurs
    ck_assert_str_eq(ms_get_env_value(env_list, "USER", 0), "gmarquis");
    ck_assert_str_eq(ms_get_env_value(env_list, "PATH", 0), "/usr/bin");
    ck_assert_str_eq(ms_get_env_value(env_list, "SHELL", 0), "/bin/bash");

    // Libération de la liste d'environnement
    free_env_list(env_list);
}
END_TEST

// Test pour vérifier que ms_get_env_value retourne NULL pour une variable inexistante
START_TEST(test_ms_get_env_value_non_existing)
{
    // Création de la liste des variables d'environnement
    t_env *env_list = create_env_node("USER", "gmarquis");
    env_list->next = create_env_node("PATH", "/usr/bin");

    // Vérification pour une variable qui n'existe pas
    ck_assert_ptr_null(ms_get_env_value(env_list, "HOME", 0));

    // Libération de la liste d'environnement
    free_env_list(env_list);
}
END_TEST

// Suite de tests pour ms_get_env_value
static Suite *ms_get_env_value_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_get_env_value");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_get_env_value_existing);
    tcase_add_test(tc_core, test_ms_get_env_value_non_existing);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_get_env_value_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

