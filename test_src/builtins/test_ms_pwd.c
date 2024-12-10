/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:27:58 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/10 10:21:42 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../../include/builtins.h"

#define STDERR_BUFFER_SIZE 1024
#define STDOUT_BUFFER_SIZE 1024

static t_gc *g_garbage_collector;  // Déclaration globale du garbage collector

typedef struct s_testresult
{
    int exit_code;
    char stdout_output[STDOUT_BUFFER_SIZE];
    char stderr_output[STDERR_BUFFER_SIZE];
} t_testresult;

// Fonction de configuration (setup)
static void setup(void)
{
    // Initialiser le garbage collector avant chaque test
    g_garbage_collector = gc_init();
}

// Fonction de nettoyage (teardown)
static void teardown(void)
{
    // Nettoyer le garbage collector après chaque test
    gc_cleanup(g_garbage_collector);
}

static t_testresult execute_and_capture(void)
{
    t_testresult result;
    int stdout_pipefd[2];
    int stderr_pipefd[2];
    pid_t pid;
    int status;
    ssize_t bytes_read;

    memset(result.stdout_output, 0, STDOUT_BUFFER_SIZE); // Initialisation des buffers
    memset(result.stderr_output, 0, STDERR_BUFFER_SIZE);

    // Créer des pipes pour capturer stdout et stderr
    if (pipe(stdout_pipefd) == -1 || pipe(stderr_pipefd) == -1)
    {
        perror("Erreur lors de la création des pipes");
        result.exit_code = -1;
        strncpy(result.stderr_output, "Erreur lors de la création des pipes\n", STDERR_BUFFER_SIZE - 1);
        return result;
    }

    gc_fd_register(stdout_pipefd[0], g_garbage_collector);
    gc_fd_register(stdout_pipefd[1], g_garbage_collector);
    gc_fd_register(stderr_pipefd[0], g_garbage_collector);
    gc_fd_register(stderr_pipefd[1], g_garbage_collector);

    pid = fork();
    if (pid == -1)
    {
        perror("Erreur lors du fork");
        result.exit_code = -1;
        strncpy(result.stderr_output, "Erreur lors du fork\n", STDERR_BUFFER_SIZE - 1);
        return result;
    }
    else if (pid == 0)
    {
        // Processus enfant
        close(stdout_pipefd[0]); // Fermer les extrémités de lecture non utilisées
        close(stderr_pipefd[0]);

        if (dup2(stdout_pipefd[1], STDOUT_FILENO) == -1 || dup2(stderr_pipefd[1], STDERR_FILENO) == -1)
        {
            perror("Erreur lors de dup2");
            exit(EXIT_FAILURE);
        }

        close(stdout_pipefd[1]); // Fermer les extrémités originales d'écriture
        close(stderr_pipefd[1]);

        int exit_code = ms_pwd(1);
        exit(exit_code);
    }
    else
    {
        // Processus parent
        close(stdout_pipefd[1]); // Fermer les extrémités d'écriture non utilisées
        close(stderr_pipefd[1]);

        waitpid(pid, &status, 0);
        result.exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

        bytes_read = read(stdout_pipefd[0], result.stdout_output, STDOUT_BUFFER_SIZE - 1);
        result.stdout_output[bytes_read >= 0 ? bytes_read : 0] = '\0';

        bytes_read = read(stderr_pipefd[0], result.stderr_output, STDERR_BUFFER_SIZE - 1);
        result.stderr_output[bytes_read >= 0 ? bytes_read : 0] = '\0';

        close(stdout_pipefd[0]);
        close(stderr_pipefd[0]);

        return result;
    }
}

//_____________________________test_ms_pwd_________________________________________________

START_TEST(test_ms_pwd_valid)
{
    t_testresult result = execute_and_capture();

    // Vérifier que le code de sortie est 0 (succès)
    ck_assert_int_eq(result.exit_code, 0);
 
    // Vérifier que stdout contient un chemin (getcwd ne devrait pas renvoyer d'erreur)
    ck_assert_str_ne(result.stdout_output, ""); 

    // Vérifier qu'il n'y a pas d'erreur sur stderr
    ck_assert_str_eq(result.stderr_output, ""); 
}
END_TEST

static Suite    *ms_pwd_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_pwd");
    tc_core = tcase_create("Core");

    // Ajouter les fonctions setup et teardown pour le garbage collector
    tcase_add_checked_fixture(tc_core, setup, teardown);

    tcase_add_test(tc_core, test_ms_pwd_valid);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_pwd_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

