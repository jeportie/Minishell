/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_syntax_error.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:00:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/29 18:23:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../../include/syntax.h"

#define STDERR_BUFFER_SIZE 1024

static t_gc *g_garbage_collector;  // Déclaration globale du garbage collector

typedef struct s_testresult
{
    int exit_code;
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

static t_testresult execute_and_capture(const char *input)
{
    t_testresult result;
    int pipefd[2];
    pid_t pid;
    int status;
    ssize_t bytes_read;

    memset(result.stderr_output, 0, STDERR_BUFFER_SIZE); // Initialisation du buffer d'erreur

    // Créer un pipe pour capturer stderr
    if (pipe(pipefd) == -1)
    {
        perror("Erreur lors de la création du pipe");
        result.exit_code = -1;
        strncpy(result.stderr_output, "Erreur lors de la création du pipe\n", STDERR_BUFFER_SIZE - 1);
        return result;
    }

    gc_fd_register(pipefd[0], g_garbage_collector);
    gc_fd_register(pipefd[1], g_garbage_collector);

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
        close(pipefd[0]); // Fermer l'extrémité de lecture non utilisée

        if (dup2(pipefd[1], STDERR_FILENO) == -1)
        {
            perror("Erreur lors de dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]); // Fermer l'extrémité d'écriture originale

        int exit_code = ms_syntax_error(input);
        exit(exit_code);
    }
    else
    {
        close(pipefd[1]); // Fermer l'extrémité d'écriture non utilisée

        waitpid(pid, &status, 0);
        result.exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

        bytes_read = read(pipefd[0], result.stderr_output, STDERR_BUFFER_SIZE - 1);
        result.stderr_output[bytes_read >= 0 ? bytes_read : 0] = '\0';
        close(pipefd[0]);

        return result;
    }
}

//_____________________________valid_paranthese_________________________________________________

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
    t_testresult result = execute_and_capture("ls | (cat)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée valide
    ck_assert_str_eq(result.stderr_output, ""); // Pas de message d'erreur attendu pour une entrée valide
}
END_TEST

START_TEST(test_ms_syntax_error_valid_p_2)
{
    t_testresult result = execute_and_capture("ls -la > file && (grep x < file | cat -e >> file) && cat file");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée valide
    ck_assert_str_eq(result.stderr_output, ""); // Pas de message d'erreur attendu pour une entrée valide
}
END_TEST

START_TEST(test_ms_syntax_error_valid_p_3)
{
    t_testresult result = execute_and_capture("echo 'valid' || (echo \"valid\" | cat -e)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée valide
    ck_assert_str_eq(result.stderr_output, ""); // Pas de message d'erreur attendu pour une entrée valide
}
END_TEST

START_TEST(test_ms_syntax_error_valid_p_4)
{
    t_testresult result = execute_and_capture("(echo '(\"(valid)\")')");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée valide
    ck_assert_str_eq(result.stderr_output, ""); // Pas de message d'erreur attendu pour une entrée valide
}
END_TEST

//______________________________invalide_parenthese_________________________________________________

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
    t_testresult result = execute_and_capture("ls)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_2)
{
    t_testresult result = execute_and_capture("ls (cat)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `cat'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_3)
{
    t_testresult result = execute_and_capture("()");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_4)
{
    t_testresult result = execute_and_capture("(|cat)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
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
    t_testresult result = execute_and_capture("(cat >)");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `)'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_7)
{
    t_testresult result = execute_and_capture("(echo ((cat)invalid))");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `('\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_p_8)
{
    t_testresult result = execute_and_capture("(echo (invalid(cat)))");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `invalid'\n"); // Message d'erreur attendu
}
END_TEST

//_____________________________valid_operator_______________________________________________________

START_TEST(test_ms_syntax_error_valid_op_0)
{
    t_testresult result = execute_and_capture("ls | pwd");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, ""); // Message d'erreur attendu
}
END_TEST

//_____________________________invalid_operator_______________________________________________________

START_TEST(test_ms_syntax_error_invalid_op_0)
{
    t_testresult result = execute_and_capture("||");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `||'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_1)
{
    t_testresult result = execute_and_capture("ls &&");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `&&'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_2)
{
    t_testresult result = execute_and_capture("| ls");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_op_3)
{
    t_testresult result = execute_and_capture("ls || && cat");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `&&'\n"); // Message d'erreur attendu
}
END_TEST

//________________________________valid_redirection________________________________________________

START_TEST(test_ms_syntax_error_valid_r_0)
{
    t_testresult result = execute_and_capture("ls > file");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, ""); // Message d'erreur attendu
}
END_TEST

//________________________________invalid_redirection_______________________________________________

START_TEST(test_ms_syntax_error_invalid_r_0)
{
    t_testresult result = execute_and_capture("ls >");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `newline'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_r_1)
{
    t_testresult result = execute_and_capture("><");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `<'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_r_2)
{
    t_testresult result = execute_and_capture(">> |");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `|'\n"); // Message d'erreur attendu
}
END_TEST

//__________________________valid_simple_quote_________________________________

START_TEST(test_ms_syntax_error_valid_sq_0)
{
    t_testresult result = execute_and_capture("echo \'valid\'");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, ""); // Message d'erreur attendu
}
END_TEST

//__________________________invalid_simple_quote________________________________

START_TEST(test_ms_syntax_error_invalid_sq_0)
{
    t_testresult result = execute_and_capture("echo \'invalide");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `''\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_sq_1)
{
    t_testresult result = execute_and_capture("\'\"invalide\"");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `''\n"); // Message d'erreur attendu
}
END_TEST

//__________________________valid_double_quote_________________________________

START_TEST(test_ms_syntax_error_valid_dq_0)
{
    t_testresult result = execute_and_capture("echo \"valid\"");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 0); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, ""); // Message d'erreur attendu
}
END_TEST

//__________________________invalid_double_quote________________________________

START_TEST(test_ms_syntax_error_invalid_dq_0)
{
    t_testresult result = execute_and_capture("echo \"invalide");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `\"'\n"); // Message d'erreur attendu
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_dq_1)
{
    t_testresult result = execute_and_capture("\"\'invalide\'");

    // Assertions pour vérifier le code de sortie et le message d'erreur attendu
    ck_assert_int_eq(result.exit_code, 1); // Code de sortie attendu pour une entrée invalide
    ck_assert_str_eq(result.stderr_output, "Minishell: syntax error near unexpected token `\"'\n"); // Message d'erreur attendu
}
END_TEST

static Suite    *ms_syntax_error_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_syntax_error");
    tc_core = tcase_create("Core");

    // Ajouter les fonctions setup et teardown pour le garbage collector
    tcase_add_checked_fixture(tc_core, setup, teardown);

    tcase_add_test(tc_core, test_ms_syntax_error_valid_p_0);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_p_1);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_p_2);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_p_4);

    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_0);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_1);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_2);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_3);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_4);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_5);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_6);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_7);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_p_8);


    tcase_add_test(tc_core, test_ms_syntax_error_valid_op_0);

    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_0);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_1);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_2);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_op_3);


    tcase_add_test(tc_core, test_ms_syntax_error_valid_r_0);

    tcase_add_test(tc_core, test_ms_syntax_error_invalid_r_0);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_r_1);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_r_2);


    tcase_add_test(tc_core, test_ms_syntax_error_valid_sq_0);

    tcase_add_test(tc_core, test_ms_syntax_error_invalid_sq_0);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_sq_1);

    tcase_add_test(tc_core, test_ms_syntax_error_valid_dq_0);

    tcase_add_test(tc_core, test_ms_syntax_error_invalid_dq_0);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_dq_1);

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

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
