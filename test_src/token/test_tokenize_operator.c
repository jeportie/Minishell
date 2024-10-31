/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenize_operator.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:29:53 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 14:23:59 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"
#include <check.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

START_TEST(test_tokenize_operator_basic)
{
    t_gc *gcl = gc_init();
    const char *input = "&&";
    t_token *token = tokenize_operator(&input, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_AND);
    ck_assert_str_eq(token->token, "&&");
    ck_assert_ptr_null(token->next);

    const char *input2 = "|";
    token = tokenize_operator(&input2, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_PIPE);
    ck_assert_str_eq(token->token, "|");
    ck_assert_ptr_null(token->next);

    const char *input3 = "||";
    token = tokenize_operator(&input3, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_OR);
    ck_assert_str_eq(token->token, "||");
    ck_assert_ptr_null(token->next);

    const char *input4 = "<<";
    token = tokenize_operator(&input4, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_REDIRECTION);
    ck_assert_str_eq(token->token, "<<");
    ck_assert_ptr_null(token->next);

    const char *input5 = "<";
    token = tokenize_operator(&input5, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_REDIRECTION);
    ck_assert_str_eq(token->token, "<");
    ck_assert_ptr_null(token->next);

    const char *input6 = ">>";
    token = tokenize_operator(&input6, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_REDIRECTION);
    ck_assert_str_eq(token->token, ">>");
    ck_assert_ptr_null(token->next);

    const char *input7 = ">";
    token = tokenize_operator(&input7, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_REDIRECTION);
    ck_assert_str_eq(token->token, ">");
    ck_assert_ptr_null(token->next);

    gc_cleanup(gcl);
    free(gcl);
}
END_TEST

START_TEST(test_tokenize_invalid_operator)
{
    t_gc *gcl = gc_init();
    const char *input = "&";
    t_token *token = tokenize_operator(&input, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_WORD);
    ck_assert_str_eq(token->token, "&");
    ck_assert_ptr_null(token->next);

    gc_cleanup(gcl);
    free(gcl);
}
END_TEST

START_TEST(test_tokenize_operator_null_input)
{
    t_gc *gcl = gc_init();
    const char *input = NULL;

    int result = 0;
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        t_token *token = tokenize_operator(&input, gcl);
        if (token == NULL)
            exit(EXIT_SUCCESS); // No segfault, exit normally
        else
            exit(EXIT_FAILURE);
    }
    else if (pid > 0) // Parent process
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV)
        {
            result = -1; // Segfault detected
        }
        else if (WIFEXITED(status))
        {
            result = WEXITSTATUS(status);
        }
    }
    ck_assert_int_ne(result, -1); // Check that no segfault occurred

    gc_cleanup(gcl);
    free(gcl);
}
END_TEST

Suite *tokenize_word_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("tokenize_operator");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_tokenize_operator_basic);
    tcase_add_test(tc_core, test_tokenize_invalid_operator);
    tcase_add_test(tc_core, test_tokenize_operator_null_input);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = tokenize_word_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

