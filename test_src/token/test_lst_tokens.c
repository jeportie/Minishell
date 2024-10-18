/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lst_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:45:22 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/18 14:38:17 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"
#include <check.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


START_TEST(test_create_token)
{
    t_gc *gcl = gc_init();
    t_token *token = create_token(TOKEN_WORD, "echo", gcl);

    ck_assert_ptr_nonnull(token);
    ck_assert_int_eq(token->type, TOKEN_WORD);
    ck_assert_str_eq(token->token, "echo");
    ck_assert_ptr_null(token->next);

    gc_cleanup(gcl);
    free(gcl);
}
END_TEST

START_TEST(test_add_token)
{
    t_gc *gcl = gc_init();
    t_token *head = NULL;
    t_token *token1 = create_token(TOKEN_WORD, "echo", gcl);
    t_token *token2 = create_token(TOKEN_OPERATOR, "&&",gcl);

    add_token(&head, token1, gcl);
    ck_assert_ptr_nonnull(head);
    ck_assert_str_eq(head->token, "echo");
    ck_assert_ptr_null(head->next);

    add_token(&head, token2, gcl);
    ck_assert_ptr_nonnull(head->next);
    ck_assert_str_eq(head->next->token, "&&");
    ck_assert_ptr_null(head->next->next);

    gc_cleanup(gcl);
    free(gcl);
}
END_TEST

START_TEST(test_add_token_null_head)
{
    t_gc *gcl = gc_init();
    t_token *token = create_token(TOKEN_WORD, "echo", gcl);

    // Expecting the function to handle NULL gracefully without segfaulting
    int result = 0;
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        add_token(NULL, token, gcl);
        exit(EXIT_SUCCESS); // No segfault, exit normally
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

START_TEST(test_add_token_null_token)
{
    t_gc *gcl = gc_init();
    t_token *head = NULL;

    add_token(&head, NULL, gcl);
    ck_assert_ptr_null(head); // Head should remain NULL since no token was added

    gc_cleanup(gcl);
    free(gcl);
}
END_TEST

START_TEST(test_add_multiple_tokens)
{
    t_gc *gcl = gc_init();
    t_token *head = NULL;
    t_token *token1 = create_token(TOKEN_WORD, "echo", gcl);
    t_token *token2 = create_token(TOKEN_OPERATOR, "&&", gcl);
    t_token *token3 = create_token(TOKEN_WORD, "ls", gcl);

    add_token(&head, token1, gcl);
    add_token(&head, token2, gcl);
    add_token(&head, token3, gcl);

    ck_assert_ptr_nonnull(head);
    ck_assert_str_eq(head->token, "echo");
    ck_assert_ptr_nonnull(head->next);
    ck_assert_str_eq(head->next->token, "&&");
    ck_assert_ptr_nonnull(head->next->next);
    ck_assert_str_eq(head->next->next->token, "ls");
    ck_assert_ptr_null(head->next->next->next);

    gc_cleanup(gcl);
    free(gcl);
}
END_TEST

Suite *lst_tokens_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("lst_tokens");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_create_token);
    tcase_add_test(tc_core, test_add_token);
    tcase_add_test(tc_core, test_add_token_null_head);
    tcase_add_test(tc_core, test_add_token_null_token);
    tcase_add_test(tc_core, test_add_multiple_tokens);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = lst_tokens_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

