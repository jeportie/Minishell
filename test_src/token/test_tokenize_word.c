/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenize_word.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:30:00 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 14:12:06 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"
#include <check.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

START_TEST(test_tokenize_word_basic)
{
    t_gc *gcl = gc_init();
    const char *input = "echo";
    t_token *token = tokenize_word(&input, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_WORD);
    ck_assert_str_eq(token->token, "echo");
    ck_assert_ptr_null(token->next);

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_tokenize_word_with_whitespace)
{
    t_gc *gcl = gc_init();
    const char *input = "  ls  ";
    skip_whitespace(&input);
    t_token *token = tokenize_word(&input, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_WORD);
    ck_assert_str_eq(token->token, "ls");
    ck_assert_ptr_null(token->next);

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_tokenize_word_empty_input)
{
    t_gc *gcl = gc_init();
    const char *input = "";
    t_token *token = tokenize_word(&input, gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_WORD);
    ck_assert_str_eq(token->token, "");
    ck_assert_ptr_null(token->next);

    gc_cleanup(gcl);
}
END_TEST

START_TEST(test_tokenize_word_null_input)
{
    t_gc *gcl = gc_init();
    const char *input = NULL;

    int result = 0;
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        t_token *token = tokenize_word(&input, gcl);
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
}
END_TEST

Suite *tokenize_word_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("tokenize_word");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_tokenize_word_basic);
    tcase_add_test(tc_core, test_tokenize_word_with_whitespace);
    tcase_add_test(tc_core, test_tokenize_word_empty_input);
    tcase_add_test(tc_core, test_tokenize_word_null_input);
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

