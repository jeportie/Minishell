/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_set_env_value.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:22:20 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 22:43:54 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <string.h>
#include "../../include/env_value.h"

void cleanup_test_shell(t_shell *shell) {
    gc_cleanup(shell->gcl); // Nettoyage du garbage collector
    free(shell->gcl);
}

void test_add_new_env_value(int argc, char **argv, char **envp) {
    t_shell shell = ms_init_shell(argc, argv, envp);

    ms_set_env_value(&shell, "USE", "1234");

    // Vérifie si la nouvelle variable a été ajoutée
    t_env *env = shell.env_data->env;
    bool found = false;
    while (env) {
        if (strcmp(env->var, "USE") == 0) {
            assert(strcmp(env->value, "1234") == 0);
            found = true;
            break;
        }
        env = env->next;
    }
    assert(found && "La variable USE n'a pas été ajoutée correctement");

    cleanup_test_shell(&shell);
}

void test_update_existing_env_value(int argc, char **argv, char **envp) {
    t_shell shell = ms_init_shell(argc, argv, envp);

    ms_set_env_value(&shell, "USER", "1234");

    // Vérifie si la valeur existante a été mise à jour
    t_env *env = shell.env_data->env;
    bool found = false;
    while (env) {
        if (strcmp(env->var, "USER") == 0) {
            assert(strcmp(env->value, "1234") == 0);
            found = true;
            break;
        }
        env = env->next;
    }
    assert(found && "La variable USER n'a pas été mise à jour correctement");

    cleanup_test_shell(&shell);
}

int main(int argc, char **argv, char **envp) {
    char *args[] = {"./minishell", NULL};
    test_add_new_env_value(1, args, envp);
    test_update_existing_env_value(1, args, envp);

    printf("Tous les tests sont réussis.\n");
    return 0;
}
