/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_syntax_error.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:00:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/28 16:08:47 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../../include/syntax.h"

#define STDERR_BUFFER_SIZE 1024

typedef struct s_testresult
{
    int     exit_code;
    char    stderr_output[STDERR_BUFFER_SIZE];
}               t_testresult;

t_testresult    execute_and_capture(const char *input)
{
    t_testresult    result;
    int             pipefd[2];
    pid_t           pid;
    int             status;
    ssize_t         bytes_read;

    // Initialiser le buffer de sortie d'erreur
    memset(result.stderr_output, 0, STDERR_BUFFER_SIZE);

    // Créer un pipe pour capturer stderr
    if (pipe(pipefd) == -1)
    {
        perror("Erreur lors de la création du pipe");
        result.exit_code = -1;
        strncpy(result.stderr_output, "Erreur lors de la création du pipe\n", STDERR_BUFFER_SIZE - 1);
        return result;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("Erreur lors du fork");
        result.exit_code = -1;
        strncpy(result.stderr_output, "Erreur lors du fork\n", STDERR_BUFFER_SIZE - 1);
        close(pipefd[0]);
        close(pipefd[1]);
        return result;
    }
    else if (pid == 0)
    {
        // Processus enfant
        close(pipefd[0]); // Fermer l'extrémité de lecture non utilisée

        // Rediriger stderr vers le pipe
        if (dup2(pipefd[1], STDERR_FILENO) == -1)
        {
            perror("Erreur lors de dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]); // Fermer l'extrémité d'écriture originale

        // Exécuter la fonction
        int exit_code = ms_syntax_error(input);

        // Sortir avec le code de sortie de ms_syntax_error
        exit(exit_code);
    }
    else
    {
        // Processus parent
        close(pipefd[1]); // Fermer l'extrémité d'écriture non utilisée

        // Attendre la fin du processus enfant et obtenir le code de sortie
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            result.exit_code = WEXITSTATUS(status);
        else
            result.exit_code = -1; // Indique que le processus enfant s'est terminé anormalement

        // Lire la sortie sur stderr depuis le pipe
        bytes_read = read(pipefd[0], result.stderr_output, STDERR_BUFFER_SIZE - 1);
        if (bytes_read >= 0)
            result.stderr_output[bytes_read] = '\0'; // Terminer la chaîne avec un caractère nul
        else
            result.stderr_output[0] = '\0';
        close(pipefd[0]);
        return result;
    }
}

//______________________________________________________________________________

START_TEST(test_ms_syntax_error_valid_p_0)
{
    t_testresult result = execute_and_capture("(ls)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée valide
    ck_assert_str_eq(result.stderr_output, ""); // Pas de message d'erreur attendu pour une entrée valide
}
END_TEST

START_TEST(test_ms_syntax_error_valid_p_1)
{
    t_testresult result = execute_and_capture("ls|(cat)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée valide
    ck_assert_str_eq(result.stderr_output, ""); // Pas de message d'erreur attendu pour une entrée valide
}
END_TEST

//_______________________________________________________________________________

START_TEST(test_ms_syntax_error_invalid_p_0)
{
    t_testresult result = execute_and_capture("(ls");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `('\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_1)
{
    t_testresult result = execute_and_capture("(ls\")\"");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `('\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_2)
{
    t_testresult result = execute_and_capture("ls)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_3)
{
    t_testresult result = execute_and_capture("ls (cat)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `cat'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_4)
{
    t_testresult result = execute_and_capture("()");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_5)
{
    t_testresult result = execute_and_capture("(cat|)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_6)
{
    t_testresult result = execute_and_capture("(|cat)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_7)
{
    t_testresult result = execute_and_capture("(cat||)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_8)
{
    t_testresult result = execute_and_capture("(||cat)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `||'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_9)
{
    t_testresult result = execute_and_capture("(cat&&)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_10)
{
    t_testresult result = execute_and_capture("(&&cat)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `&&'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_11)
{
    t_testresult result = execute_and_capture("(cat >)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_12)
{
    t_testresult result = execute_and_capture("(cat >>)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_13)
{
    t_testresult result = execute_and_capture("(cat <)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_14)
{
    t_testresult result = execute_and_capture("(cat <<)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

//____________________________________________________________________________________

START_TEST(test_ms_syntax_error_valid_op_0)
{
    t_testresult result = execute_and_capture("ls | pwd");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, ""); // Message d'erreur attendu
}
END_TEST

//____________________________________________________________________________________

START_TEST(test_ms_syntax_error_invalid_op_0)
{
    t_testresult result = execute_and_capture("|");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_1)
{
    t_testresult result = execute_and_capture("||");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `||'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_2)
{
    t_testresult result = execute_and_capture("&&");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `&&'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_3)
{
    t_testresult result = execute_and_capture("ls |");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_4)
{
    t_testresult result = execute_and_capture("| ls");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_5)
{
    t_testresult result = execute_and_capture("ls ||");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `||'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_6)
{
    t_testresult result = execute_and_capture("|| ls");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `||'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_7)
{
    t_testresult result = execute_and_capture("ls &&");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `&&'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_8)
{
    t_testresult result = execute_and_capture("&& ls");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `&&'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_9)
{
    t_testresult result = execute_and_capture("ls | | cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_10)
{
    t_testresult result = execute_and_capture("ls | || cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `||'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_11)
{
    t_testresult result = execute_and_capture("ls | && cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `&&'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_12)
{
    t_testresult result = execute_and_capture("ls || | cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_13)
{
    t_testresult result = execute_and_capture("ls || || cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `||'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_14)
{
    t_testresult result = execute_and_capture("ls || && cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `&&'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_15)
{
    t_testresult result = execute_and_capture("ls && | cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_16)
{
    t_testresult result = execute_and_capture("ls && || cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `||'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_17)
{
    t_testresult result = execute_and_capture("ls && && cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `&&'\n"); // Message d'erreur attendu
}
END_TEST

Suite *ms_syntax_error_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_syntax_error");
    tc_core = tcase_create("Core");

    // Désactiver le fork automatique de Check pour ce cas de test
    tcase_set_timeout(tc_core, 0);

    tcase_add_test(tc_core, test_ms_syntax_error_valid_p_0);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_p_1);

    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_0);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_1);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_2);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_3);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_4);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_5);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_6);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_7);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_8);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_9);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_10);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_11);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_12);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_13);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_14);


    tcase_add_test(tc_core, test_ms_syntax_error_valid_op_0);

    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_0);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_1);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_2);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_3);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_4);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_5);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_6);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_7);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_8);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_9);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_10);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_11);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_12);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_13);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_14);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_15);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_16);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_17);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_syntax_error_suite();
    sr = srunner_create(s);

    // Exécuter tous les tests
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
