/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_syntax_error.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:00:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/25 12:04:38 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../include/syntax.h"

/* Helper function to test functions that call exit() */
int test_ms_syntax_error_exit(const char *input)
{
    int error;
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        error = ms_syntax_error(input);
        exit(error); // If exit isn't called, exit normally
    }
    else if (pid > 0) // Parent process
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status); // Return the exit code
        }
        else
        {
            return -1; // Error occurred
        }
    }
    return -1; // Fork failed
}

START_TEST(test_ms_syntax_error_valid_input_0)
{
    int exit_code = test_ms_syntax_error_exit("valid input");
    ck_assert_int_eq(exit_code, 0); // Expect success with valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_1)
{
    int exit_code = test_ms_syntax_error_exit("\'valid input\'");
    ck_assert_int_eq(exit_code, 0); // Expect success with valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_2)
{
    int exit_code = test_ms_syntax_error_exit("\"valid input\"");
    ck_assert_int_eq(exit_code, 0); // Expect success with valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_3)
{
    int exit_code = test_ms_syntax_error_exit("\"valid input\"\"\"");
    ck_assert_int_eq(exit_code, 0); // Expect success with valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_4)
{
    int exit_code = test_ms_syntax_error_exit("echo \"Hello World\"");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_5)
{
    int exit_code = test_ms_syntax_error_exit("ls -la | grep txt");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_6)
{
    int exit_code = test_ms_syntax_error_exit("(echo test) && (ls)");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_7)
{
    int exit_code = test_ms_syntax_error_exit("\"string with 'spaces\"");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_8)
{
    int exit_code = test_ms_syntax_error_exit("'quoted\" text'");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_9)
{
    int exit_code = test_ms_syntax_error_exit("echo \"multi 'quote' test\"");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_10)
{
    int exit_code = test_ms_syntax_error_exit("cat < file.txt > output.txt");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_11)
{
    int exit_code = test_ms_syntax_error_exit("(echo \"nested\" | grep \"nest\")");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_12)
{
    int exit_code = test_ms_syntax_error_exit("echo \"\" | cat");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_13)
{
    int exit_code = test_ms_syntax_error_exit("echo ''");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_14)
{
    int exit_code = test_ms_syntax_error_exit("echo '('");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_15)
{
    int exit_code = test_ms_syntax_error_exit("ls -la | (grep txt && echo \"done\")");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_16)
{
    int exit_code = test_ms_syntax_error_exit("(echo test) || (false')')");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_17)
{
    int exit_code = test_ms_syntax_error_exit("echo \"nested quotes 'inside'\" && ('ls')");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_18)
{
    int exit_code = test_ms_syntax_error_exit("ls && echo \"done\" || ls");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_19)
{
    int exit_code = test_ms_syntax_error_exit("echo > file.txt && cat (ls && ls > outfile) || ls");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_20)
{
    int exit_code = test_ms_syntax_error_exit("echo \"hello > world\" > outfile || ls");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_21)
{
    int exit_code = test_ms_syntax_error_exit("echo \"start && end\"");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_22)
{
    int exit_code = test_ms_syntax_error_exit("cat file.txt | grep 'pattern'");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_23)
{
    int exit_code = test_ms_syntax_error_exit("echo \"escaped \\\"quotes\\\"\"");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_24)
{
    int exit_code = test_ms_syntax_error_exit("\"nested (commands)\"");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_25)
{
    int exit_code = test_ms_syntax_error_exit("echo (test | grep 't')");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_26)
{
    int exit_code = test_ms_syntax_error_exit("echo 'multi &&| ||test'");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_27)
{
    int exit_code = test_ms_syntax_error_exit("true && false || echo \"valid\"");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_28)
{
    int exit_code = test_ms_syntax_error_exit("ls | grep \".c\" && echo \"C files\"");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_29)
{
    int exit_code = test_ms_syntax_error_exit("echo (test) | cat > output.txt");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_valid_input_30)
{
    int exit_code = test_ms_syntax_error_exit("ls -la | (echo hello)");
    ck_assert_int_eq(exit_code, 0); // Valid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_0)
{
    int exit_code = test_ms_syntax_error_exit("\"invalide input\"\'");
    ck_assert_int_eq(exit_code, 1); // Expect success with valid input
}

START_TEST(test_ms_syntax_error_invalid_input_1)
{
    int exit_code = test_ms_syntax_error_exit("\"invalide input");
    ck_assert_int_eq(exit_code, 1); // Expect success with valid input
}

START_TEST(test_ms_syntax_error_invalid_input_2)
{
    int exit_code = test_ms_syntax_error_exit("\"invalide input\'\"\'\"\'\'");
    ck_assert_int_eq(exit_code, 1); // Expect success with valid input
}

START_TEST(test_ms_syntax_error_invalid_input_3)
{
    int exit_code = test_ms_syntax_error_exit("(invalid input");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_4)
{
    int exit_code = test_ms_syntax_error_exit("echo unterminated string)");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_5)
{
    int exit_code = test_ms_syntax_error_exit("echo \"mismatch' quotes");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_6)
{
    int exit_code = test_ms_syntax_error_exit("cat < > file");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_7)
{
    int exit_code = test_ms_syntax_error_exit("echo > > file");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_8)
{
    int exit_code = test_ms_syntax_error_exit("|| ls");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_9)
{
    int exit_code = test_ms_syntax_error_exit("ls &&");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_10)
{
    int exit_code = test_ms_syntax_error_exit("echo | | grep txt");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_11)
{
    int exit_code = test_ms_syntax_error_exit("echo |");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_12)
{
    int exit_code = test_ms_syntax_error_exit("| echo");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_13)
{
    int exit_code = test_ms_syntax_error_exit("cat << >>");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_14)
{
    int exit_code = test_ms_syntax_error_exit("cat > < file.txt");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_15)
{
    int exit_code = test_ms_syntax_error_exit("echo ||");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_16)
{
    int exit_code = test_ms_syntax_error_exit("echo (");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_17)
{
    int exit_code = test_ms_syntax_error_exit(") ls ");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_18)
{
    int exit_code = test_ms_syntax_error_exit("&& echo");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_19)
{
    int exit_code = test_ms_syntax_error_exit("ls ||| ls");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_20)
{
    int exit_code = test_ms_syntax_error_exit("ls || || echo");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_21)
{
    int exit_code = test_ms_syntax_error_exit("cat <<< file");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_22)
{
    int exit_code = test_ms_syntax_error_exit("echo \"unfinished string\" && ()");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_23)
{
    int exit_code = test_ms_syntax_error_exit("echo '' || (ls >)i");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_24)
{
    int exit_code = test_ms_syntax_error_exit("(ls | echo \"broken");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_25)
{
    int exit_code = test_ms_syntax_error_exit("grep \"unterminated");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_26)
{
    int exit_code = test_ms_syntax_error_exit("echo test > outfile < infile || (ls |)");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_27)
{
    int exit_code = test_ms_syntax_error_exit("(cat < outfile) < file.txt >> ");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_28)
{
    int exit_code = test_ms_syntax_error_exit("echo ||| echo");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_29)
{
    int exit_code = test_ms_syntax_error_exit("cat >> >> file.txt");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_30)
{
    int exit_code = test_ms_syntax_error_exit("echo \"open quote");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_31)
{
    int exit_code = test_ms_syntax_error_exit("echo 'unclosed parenthesis\"");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_32)
{
    int exit_code = test_ms_syntax_error_exit("echo 'unterminated string");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_33)
{
    int exit_code = test_ms_syntax_error_exit("cat >");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_34)
{
    int exit_code = test_ms_syntax_error_exit("echo (unfinished");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_35)
{
    int exit_code = test_ms_syntax_error_exit("echo \"unterminated \"quote\"");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_36)
{
    int exit_code = test_ms_syntax_error_exit("cat <");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_37)
{
    int exit_code = test_ms_syntax_error_exit("echo (mismatched parenthesis");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_38)
{
    int exit_code = test_ms_syntax_error_exit("echo \"nested 'quotes\" inside'");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_39)
{
    int exit_code = test_ms_syntax_error_exit("cat << << file.txt");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_40)
{
    int exit_code = test_ms_syntax_error_exit("echo &&& echo");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_41)
{
    int exit_code = test_ms_syntax_error_exit("echo ()");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_42)
{
    int exit_code = test_ms_syntax_error_exit("echo (nested | wrong");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_43)
{
    int exit_code = test_ms_syntax_error_exit("(echo \"missing end");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_44)
{
    int exit_code = test_ms_syntax_error_exit("echo \"unclosed quote");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_45)
{
    int exit_code = test_ms_syntax_error_exit("echo \"open bracket (");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_46)
{
    int exit_code = test_ms_syntax_error_exit("echo 'unterminated command");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_47)
{
    int exit_code = test_ms_syntax_error_exit("cat <");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_48)
{
    int exit_code = test_ms_syntax_error_exit("echo > > file.txt");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

START_TEST(test_ms_syntax_error_invalid_input_49)
{
    int exit_code = test_ms_syntax_error_exit("echo \"unterminated 'quote");
    ck_assert_int_eq(exit_code, 1); // Invalid input
}
END_TEST

Suite *ms_syntax_error_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_syntax_error");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_0);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_1);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_2);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_3);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_4);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_5);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_6);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_7);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_8);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_9);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_10);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_11);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_12);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_13);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_14);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_15);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_16);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_17);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_18);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_19);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_20);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_21);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_22);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_23);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_24);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_25);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_26);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_27);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_28);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_29);
    tcase_add_test(tc_core, test_ms_syntax_error_valid_input_30);
    
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_0);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_1);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_2);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_3);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_4);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_5);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_6);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_7);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_8);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_9);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_10);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_11);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_12);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_13);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_14);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_15);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_16);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_17);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_18);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_19);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_20);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_21);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_22);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_23);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_24);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_25);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_26);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_27);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_28);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_29);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_30);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_31);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_32);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_33);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_34);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_35);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_36);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_37);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_38);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_39);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_40);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_41);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_42);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_43);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_44);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_45);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_46);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_47);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_48);
    tcase_add_test(tc_core, test_ms_syntax_error_invalid_input_49);
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
