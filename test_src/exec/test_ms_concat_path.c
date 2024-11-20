/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_concat_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:35:57 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 13:41:55 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/exec.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

static t_gc *gcl;

void setup(void)
{
    gcl = gc_init();
    if (!gcl)
    {
        perror("gc_init");
        exit(EXIT_FAILURE);
    }
}

void teardown(void)
{
    gc_cleanup(gcl);
}

/* Test Cases */

/* Test 1: Typical case with normal inputs */
START_TEST(test_ms_concat_path_normal)
{
    const char *path = "/usr/bin";
    const char *command = "ls";

    char *result = ms_concat_path(path, command, gcl);

    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "/usr/bin/ls");
}
END_TEST

/* Test 2: Path with trailing slash */
START_TEST(test_ms_concat_path_trailing_slash)
{
    const char *path = "/usr/bin/";
    const char *command = "ls";

    char *result = ms_concat_path(path, command, gcl);

    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "/usr/bin//ls");
}
END_TEST

/* Test 3: Empty path string */
START_TEST(test_ms_concat_path_empty_path)
{
    const char *path = "";
    const char *command = "ls";

    char *result = ms_concat_path(path, command, gcl);

    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "/ls");
}
END_TEST

/* Test 4: Empty command string */
START_TEST(test_ms_concat_path_empty_command)
{
    const char *path = "/usr/bin";
    const char *command = "";

    char *result = ms_concat_path(path, command, gcl);

    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "/usr/bin/");
}
END_TEST

/* Test 5: Both path and command are empty */
START_TEST(test_ms_concat_path_empty_both)
{
    const char *path = "";
    const char *command = "";

    char *result = ms_concat_path(path, command, gcl);

    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "/");
}
END_TEST

/* Test 6: Path is NULL */
START_TEST(test_ms_concat_path_null_path)
{
    const char *path = NULL;
    const char *command = "ls";

    pid_t pid = fork();
    ck_assert_int_ne(pid, -1);

    if (pid == 0)
    {
        /* Child process */
        char *result = ms_concat_path(path, command, gcl);
        /* Should not reach here */
        gc_cleanup(gcl);
        exit(EXIT_FAILURE);
    }
    else
    {
        /* Parent process */
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            ck_assert_int_eq(exit_status, EXIT_FAILURE);
        }
        else if (WIFSIGNALED(status))
        {
            int term_sig = WTERMSIG(status);
            ck_assert_msg(term_sig != SIGSEGV, "Segmentation fault occurred with NULL path.");
            ck_assert_msg(0, "Child process terminated by signal %d", term_sig);
        }
        else
            ck_assert_msg(0, "Child process did not exit normally.");
    }
}
END_TEST

/* Test 7: Command is NULL */
START_TEST(test_ms_concat_path_null_command)
{
    const char *path = "/usr/bin";
    const char *command = NULL;

    pid_t pid = fork();
    ck_assert_int_ne(pid, -1);

    if (pid == 0)
    {
        /* Child process */
        char *result = ms_concat_path(path, command, gcl);
        /* Should not reach here */
        gc_cleanup(gcl);
        exit(EXIT_FAILURE);
    }
    else
    {
        /* Parent process */
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            ck_assert_int_eq(exit_status, EXIT_FAILURE);
        }
        else if (WIFSIGNALED(status))
        {
            int term_sig = WTERMSIG(status);
            ck_assert_msg(term_sig != SIGSEGV, "Segmentation fault occurred with NULL command.");
            ck_assert_msg(0, "Child process terminated by signal %d", term_sig);
        }
        else
            ck_assert_msg(0, "Child process did not exit normally.");
    }
}
END_TEST

/* Test 8: Path with special characters */
START_TEST(test_ms_concat_path_special_chars)
{
    const char *path = "/usr/bin";
    const char *command = "my command";

    char *result = ms_concat_path(path, command, gcl);

    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "/usr/bin/my command");
}
END_TEST

/* Test 9: Very long path and command */
START_TEST(test_ms_concat_path_long_inputs)
{
    char path[PATH_MAX];
    char command[NAME_MAX];
    memset(path, 'a', PATH_MAX - 1);
    path[PATH_MAX - 1] = '\0';
    memset(command, 'b', NAME_MAX - 1);
    command[NAME_MAX - 1] = '\0';

    pid_t pid = fork();
    ck_assert_int_ne(pid, -1);

    if (pid == 0)
    {
        /* Child process */
        char *result = ms_concat_path(path, command, gcl);
        /* Depending on system limits, this may fail */
        if (result)
            gc_cleanup(gcl);
        exit(EXIT_SUCCESS);
    }
    else
    {
        /* Parent process */
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            ck_assert_int_eq(exit_status, EXIT_SUCCESS);
        }
        else if (WIFSIGNALED(status))
        {
            int term_sig = WTERMSIG(status);
            ck_assert_msg(term_sig != SIGSEGV, "Segmentation fault occurred with long inputs.");
            ck_assert_msg(0, "Child process terminated by signal %d", term_sig);
        }
        else
            ck_assert_msg(0, "Child process did not exit normally.");
    }
}
END_TEST

/* Test 10: Path ending with multiple slashes */
START_TEST(test_ms_concat_path_multiple_trailing_slashes)
{
    const char *path = "/usr/bin///";
    const char *command = "ls";

    char *result = ms_concat_path(path, command, gcl);

    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "/usr/bin////ls");
}
END_TEST

/* Test 11: Command containing slashes */
START_TEST(test_ms_concat_path_command_with_slash)
{
    const char *path = "/usr";
    const char *command = "bin/ls";

    char *result = ms_concat_path(path, command, gcl);

    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "/usr/bin/ls");
}
END_TEST

/* Test Suite Setup */
Suite *ms_concat_path_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_concat_path");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_concat_path_normal);
    tcase_add_test(tc_core, test_ms_concat_path_trailing_slash);
    tcase_add_test(tc_core, test_ms_concat_path_empty_path);
    tcase_add_test(tc_core, test_ms_concat_path_empty_command);
    tcase_add_test(tc_core, test_ms_concat_path_empty_both);
    tcase_add_test(tc_core, test_ms_concat_path_null_path);
    tcase_add_test(tc_core, test_ms_concat_path_null_command);
    tcase_add_test(tc_core, test_ms_concat_path_special_chars);
    tcase_add_test(tc_core, test_ms_concat_path_long_inputs);
    tcase_add_test(tc_core, test_ms_concat_path_multiple_trailing_slashes);
    tcase_add_test(tc_core, test_ms_concat_path_command_with_slash);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_concat_path_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
