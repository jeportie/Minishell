/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:56:24 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 18:15:22 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/builtins.h"
#include "../../include/exec.h"

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

// Test pour afficher les variables d'environnement sans option
START_TEST(test_ms_env_no_options)
{
    t_shell shell;
    setup_shell(&shell);

    ms_set_env_value(&shell, "PATH", "/usr/bin");
    ms_set_env_value(&shell, "HOME", "/home/user");

    t_cmd_node cmd_node;
    cmd_node.argc = 1;
    char *argv[] = {"env", NULL};
    cmd_node.argv = argv;

    int result = ms_env(&cmd_node, &(t_exec_context){ .shell = &shell });

    // Vérifie que le résultat est correct (pas d'erreur)
    ck_assert_int_eq(result, 0);

    teardown_shell(&shell);
}
END_TEST

// Suite de tests pour ms_env
Suite *ms_env_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_env");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_env_no_options);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_env_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

