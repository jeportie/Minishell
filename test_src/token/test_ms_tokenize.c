/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_tokenize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:12:36 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/09 13:18:52 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// test_tokenize.c

#include <check.h>
#include <stdlib.h>
#include "../../include/tokenize.h"
#include "../../lib/libgc/include/libgc.h"

/* Test Case 1: Simple Command */
START_TEST(test_simple_command)
{
    t_gc *gcl = gc_init();
    const char *input = "ls -l /home";
    t_token *tokens = ms_tokenize(input, gcl);

    ck_assert_ptr_nonnull(tokens);
    ck_assert_int_eq(tokens->type, TOKEN_WORD);
    ck_assert_str_eq(tokens->token, "ls");

    t_token *second = tokens->next;
    ck_assert_ptr_nonnull(second);
    ck_assert_int_eq(second->type, TOKEN_WORD);
    ck_assert_str_eq(second->token, "-l");

    t_token *third = second->next;
    ck_assert_ptr_nonnull(third);
    ck_assert_int_eq(third->type, TOKEN_WORD);
    ck_assert_str_eq(third->token, "/home");

    ck_assert_ptr_null(third->next);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 2: Command with Quotes */
START_TEST(test_command_with_quotes)
{
    t_gc *gcl = gc_init();
    const char *input = "echo \"Hello World\"";
    t_token *tokens = ms_tokenize(input, gcl);

    ck_assert_ptr_nonnull(tokens);
    ck_assert_int_eq(tokens->type, TOKEN_WORD);
    ck_assert_str_eq(tokens->token, "echo");

    t_token *second = tokens->next;
    ck_assert_ptr_nonnull(second);
    ck_assert_int_eq(second->type, TOKEN_WORD);
    ck_assert_str_eq(second->token, "Hello World");

    ck_assert_ptr_null(second->next);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 3: Command with Single Quotes */
START_TEST(test_command_with_single_quotes)
{
    t_gc *gcl = gc_init();
    const char *input = "echo 'It\\'s a test'";
    t_token *tokens = ms_tokenize(input, gcl);

    ck_assert_ptr_nonnull(tokens);
    ck_assert_int_eq(tokens->type, TOKEN_WORD);
    ck_assert_str_eq(tokens->token, "echo");

    t_token *second = tokens->next;
    ck_assert_ptr_nonnull(second);
    ck_assert_int_eq(second->type, TOKEN_WORD);
    ck_assert_str_eq(second->token, "It's a test");

    ck_assert_ptr_null(second->next);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 4: Command with Redirection */
START_TEST(test_command_with_redirection)
{
    t_gc *gcl = gc_init();
    const char *input = "grep 'pattern' < input.txt > output.txt";
    t_token *tokens = ms_tokenize(input, gcl);

    // Expected tokens: grep, 'pattern', <, input.txt, >, output.txt

    t_token *current = tokens;

    // Token 1: grep
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "grep");
    current = current->next;

    // Token 2: 'pattern'
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "pattern");
    current = current->next;

    // Token 3: <
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_REDIRECTION);
    ck_assert_str_eq(current->token, "<");
    current = current->next;

    // Token 4: input.txt
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "input.txt");
    current = current->next;

    // Token 5: >
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_REDIRECTION);
    ck_assert_str_eq(current->token, ">");
    current = current->next;

    // Token 6: output.txt
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "output.txt");
    current = current->next;

    ck_assert_ptr_null(current);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 5: Command with Pipe */
START_TEST(test_command_with_pipe)
{
    t_gc *gcl = gc_init();
    const char *input = "cat file.txt | grep 'search'";
    t_token *tokens = ms_tokenize(input, gcl);

    // Expected tokens: cat, file.txt, |, grep, 'search'

    t_token *current = tokens;

    // Token 1: cat
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "cat");
    current = current->next;

    // Token 2: file.txt
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "file.txt");
    current = current->next;

    // Token 3: |
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_PIPE);
    ck_assert_str_eq(current->token, "|");
    current = current->next;

    // Token 4: grep
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "grep");
    current = current->next;

    // Token 5: 'search'
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "search");
    current = current->next;

    ck_assert_ptr_null(current);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 6: Command with Wildcards */
START_TEST(test_command_with_wildcards)
{
    t_gc *gcl = gc_init();
    const char *input = "ls *.c src/*.h";
    t_token *tokens = ms_tokenize(input, gcl);

    // Expected tokens: ls, *.c, src/*.h

    t_token *current = tokens;

    // Token 1: ls
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "ls");
    current = current->next;

    // Token 2: *.c
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "*.c");
    current = current->next;

    // Token 3: src/*.h
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "src/*.h");
    current = current->next;

    ck_assert_ptr_null(current);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 7: Command with Logical Operators */
START_TEST(test_command_with_logical_operators)
{
    t_gc *gcl = gc_init();
    const char *input = "make && make install || echo 'Build failed'";
    t_token *tokens = ms_tokenize(input, gcl);

    // Expected tokens: make, &&, make, install, ||, echo, 'Build failed'

    t_token *current = tokens;

    // Token 1: make
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "make");
    current = current->next;

    // Token 2: &&
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_AND);
    ck_assert_str_eq(current->token, "&&");
    current = current->next;

    // Token 3: make
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "make");
    current = current->next;

    // Token 4: install
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "install");
    current = current->next;

    // Token 5: ||
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_OR);
    ck_assert_str_eq(current->token, "||");
    current = current->next;

    // Token 6: echo
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "echo");
    current = current->next;

    // Token 7: 'Build failed'
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "Build failed");
    current = current->next;

    ck_assert_ptr_null(current);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 8: Command with Unmatched Quotes (Error Case) */
START_TEST(test_command_with_unmatched_quotes)
{
    t_gc *gcl = gc_init();
    const char *input = "echo \"Unmatched quote";
    t_token *tokens = ms_tokenize(input, gcl);

    ck_assert_ptr_null(tokens);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 9: Command with Background Execution */
START_TEST(test_command_with_background_execution)
{
    t_gc *gcl = gc_init();
    const char *input = "sleep 10 &";
    t_token *tokens = ms_tokenize(input, gcl);

    // Expected tokens: sleep, 10, &

    t_token *current = tokens;

    // Token 1: sleep
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "sleep");
    current = current->next;

    // Token 2: 10
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "10");
    current = current->next;

    // Token 3: &
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_BACKGROUND);
    ck_assert_str_eq(current->token, "&");
    current = current->next;

    ck_assert_ptr_null(current);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 10: Command with Subshell */
START_TEST(test_command_with_subshell)
{
    t_gc *gcl = gc_init();
    const char *input = "(echo 'Hello World')";
    t_token *tokens = ms_tokenize(input, gcl);

    // Expected tokens: (, echo, 'Hello World', )

    t_token *current = tokens;

    // Token 1: (
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_SUBSHELL_START);
    ck_assert_str_eq(current->token, "(");
    current = current->next;

    // Token 2: echo
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "echo");
    current = current->next;

    // Token 3: 'Hello World'
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "Hello World");
    current = current->next;

    // Token 4: )
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_SUBSHELL_STOP);
    ck_assert_str_eq(current->token, ")");
    current = current->next;

    ck_assert_ptr_null(current);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 11: Command with Multiple Redirections */
START_TEST(test_command_with_multiple_redirections)
{
    t_gc *gcl = gc_init();
    const char *input = "command < input.txt >> output.txt";
    t_token *tokens = ms_tokenize(input, gcl);

    // Expected tokens: command, <, input.txt, >>, output.txt

    t_token *current = tokens;

    // Token 1: command
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "command");
    current = current->next;

    // Token 2: <
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_REDIRECTION);
    ck_assert_str_eq(current->token, "<");
    current = current->next;

    // Token 3: input.txt
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "input.txt");
    current = current->next;

    // Token 4: >>
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_REDIRECTION);
    ck_assert_str_eq(current->token, ">>");
    current = current->next;

    // Token 5: output.txt
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "output.txt");
    current = current->next;

    ck_assert_ptr_null(current);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 12: Empty Input */
START_TEST(test_empty_input)
{
    t_gc *gcl = gc_init();
    const char *input = "";
    t_token *tokens = ms_tokenize(input, gcl);

    ck_assert_ptr_null(tokens);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 13: Input with Only Whitespace */
START_TEST(test_input_with_only_whitespace)
{
    t_gc *gcl = gc_init();
    const char *input = "    \t\n  ";
    t_token *tokens = ms_tokenize(input, gcl);

    ck_assert_ptr_null(tokens);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Test Case 14: Complex Command */
START_TEST(test_complex_command)
{
    t_gc *gcl = gc_init();
    const char *input = "echo \"Start\"; (cd /tmp && ls) | grep 'file' > result.txt";
    t_token *tokens = ms_tokenize(input, gcl);

    // Expected tokens: echo, "Start", ;, (, cd, /tmp, &&, ls, ), |, grep, 'file', >, result.txt

    t_token *current = tokens;

    // Token 1: echo
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "echo");
    current = current->next;

    // Token 2: "Start"
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "Start");
    current = current->next;

    // Token 3: ;
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_SEQUENCE);
    ck_assert_str_eq(current->token, ";");
    current = current->next;

    // Token 4: (
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_SUBSHELL_START);
    ck_assert_str_eq(current->token, "(");
    current = current->next;

    // Token 5: cd
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "cd");
    current = current->next;

    // Token 6: /tmp
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "/tmp");
    current = current->next;

    // Token 7: &&
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_AND);
    ck_assert_str_eq(current->token, "&&");
    current = current->next;

    // Token 8: ls
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "ls");
    current = current->next;

    // Token 9: )
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_SUBSHELL_STOP);
    ck_assert_str_eq(current->token, ")");
    current = current->next;

    // Token 10: |
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_PIPE);
    ck_assert_str_eq(current->token, "|");
    current = current->next;

    // Token 11: grep
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "grep");
    current = current->next;

    // Token 12: 'file'
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "file");
    current = current->next;

    // Token 13: >
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_REDIRECTION);
    ck_assert_str_eq(current->token, ">");
    current = current->next;

    // Token 14: result.txt
    ck_assert_ptr_nonnull(current);
    ck_assert_int_eq(current->type, TOKEN_WORD);
    ck_assert_str_eq(current->token, "result.txt");
    current = current->next;

    ck_assert_ptr_null(current);

    // Cleanup
    gc_cleanup(gcl);
}
END_TEST

/* Suite Setup */
Suite *tokenize_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Tokenize");

    /* Core test case */
    tc_core = tcase_create("Core");

    // Add test cases to the test case
    tcase_add_test(tc_core, test_simple_command);
    tcase_add_test(tc_core, test_command_with_quotes);
    tcase_add_test(tc_core, test_command_with_single_quotes);
    tcase_add_test(tc_core, test_command_with_redirection);
    tcase_add_test(tc_core, test_command_with_pipe);
    tcase_add_test(tc_core, test_command_with_wildcards);
    tcase_add_test(tc_core, test_command_with_logical_operators);
    tcase_add_test(tc_core, test_command_with_unmatched_quotes);
    tcase_add_test(tc_core, test_command_with_background_execution);
    tcase_add_test(tc_core, test_command_with_subshell);
    tcase_add_test(tc_core, test_command_with_multiple_redirections);
    tcase_add_test(tc_core, test_empty_input);
    tcase_add_test(tc_core, test_input_with_only_whitespace);
    tcase_add_test(tc_core, test_complex_command);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main Function to Run the Tests */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = tokenize_suite();
    sr = srunner_create(s);

    // srunner_run_to(sr, s, "Core", "test_simple_command"); // For debugging specific tests

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}