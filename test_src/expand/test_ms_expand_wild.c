/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_expand_wild.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:45:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/20 13:15:23 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/expand.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

static t_gc *gcl;

/* Helper function to create a temporary directory */
char *create_temp_dir(void)
{
    char template[] = "/tmp/test_ms_expand_wild_XXXXXX";
    char *temp_dir = mkdtemp(template);
    if (!temp_dir)
    {
        perror("mkdtemp");
        exit(EXIT_FAILURE);
    }
    return strdup(temp_dir);
}

/* Helper function to remove a directory recursively */
void remove_directory(const char *path)
{
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    struct dirent *entry;

    if (d)
    {
        while ((entry = readdir(d)) != NULL)
        {
            char *entry_path;
            size_t len;

            /* Skip '.' and '..' */
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            len = path_len + strlen(entry->d_name) + 2;
            entry_path = malloc(len);
            if (!entry_path)
            {
                perror("malloc");
                closedir(d);
                return;
            }

            snprintf(entry_path, len, "%s/%s", path, entry->d_name);

            if (entry->d_type == DT_DIR)
            {
                remove_directory(entry_path);
            }
            else
            {
                unlink(entry_path);
            }
            free(entry_path);
        }
        closedir(d);
    }
    rmdir(path);
}

/* Helper function to create files in the temporary directory */
void create_files(const char *dir, const char **filenames)
{
    char filepath[PATH_MAX];
    for (int i = 0; filenames[i] != NULL; i++)
    {
        snprintf(filepath, sizeof(filepath), "%s/%s", dir, filenames[i]);
        int fd = open(filepath, O_CREAT | O_WRONLY, 0644);
        if (fd == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        close(fd);
        printf("Created file: %s\n", filepath); // Debug statement
    }
}

/* Setup and Teardown functions */
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

/* Test 1: Simple pattern matching */
START_TEST(test_ms_expand_wild_simple_pattern)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    /* Change to the temporary directory */
    ck_assert_int_eq(chdir(temp_dir), 0);
    printf("Current directory after chdir: %s\n", getcwd(NULL, 0)); // Debug statement

    /* Create files */
    const char *filenames[] = {"file1.txt", "file2.txt", "test.c", "script.sh", NULL};
    create_files(temp_dir, filenames);

    /* Pattern to match */
    const char *pattern = "*.txt";

    /* Call the function */
    t_wildcard_context *matches = ms_expand_wild(pattern, gcl);

    /* Verify the matches */
    ck_assert_ptr_nonnull(matches->matches);
    ck_assert_ptr_nonnull(matches->matches[0]);
    ck_assert_ptr_nonnull(matches->matches[1]);
    ck_assert_ptr_null(matches->matches[2]);

    /* Check that matches are "file1.txt" and "file2.txt" */
    bool match1 = (strcmp(matches->matches[0], "file1.txt") == 0 || strcmp(matches->matches[1], "file1.txt") == 0);
    bool match2 = (strcmp(matches->matches[0], "file2.txt") == 0 || strcmp(matches->matches[1], "file2.txt") == 0);
    ck_assert(match1);
    ck_assert(match2);

    /* Clean up */
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 2: No matches found */
START_TEST(test_ms_expand_wild_no_matches)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    ck_assert_int_eq(chdir(temp_dir), 0);

    /* Create files */
    const char *filenames[] = {"file1.txt", "file2.txt", NULL};
    create_files(temp_dir, filenames);

    /* Pattern to match */
    const char *pattern = "*.c";

    /* Call the function */
    t_wildcard_context *matches = ms_expand_wild(pattern, gcl);

    /* Verify the matches */
    ck_assert_ptr_nonnull(matches->matches);
    ck_assert_ptr_null(matches->matches[0]); // Should be empty list

    /* Clean up */
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 3: Matching hidden files */
START_TEST(test_ms_expand_wild_hidden_files)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    ck_assert_int_eq(chdir(temp_dir), 0);
    printf("Current directory after chdir: %s\n", getcwd(NULL, 0)); // Debug statement

    /* Create files */
    const char *filenames[] = {".hidden1", ".hidden2", "visible", NULL};
    create_files(temp_dir, filenames);

    /* Pattern to match hidden files */
    const char *pattern = ".*";

    /* Call the function */
    t_wildcard_context *matches = ms_expand_wild(pattern, gcl);

    /* Verify the matches */
    ck_assert_ptr_nonnull(matches->matches);
    ck_assert_ptr_nonnull(matches->matches[0]);
    ck_assert_ptr_nonnull(matches->matches[1]);
    ck_assert_ptr_null(matches->matches[2]);

    /* Check that matches are ".hidden1" and ".hidden2" */
    bool match1 = (strcmp(matches->matches[0], ".hidden1") == 0 || strcmp(matches->matches[1], ".hidden1") == 0);
    bool match2 = (strcmp(matches->matches[0], ".hidden2") == 0 || strcmp(matches->matches[1], ".hidden2") == 0);
    ck_assert(match1);
    ck_assert(match2);

    /* Clean up */
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST


/* Test 5: Empty pattern */
START_TEST(test_ms_expand_wild_empty_pattern)
{
    /* Call the function with empty pattern */
    t_wildcard_context *matches = ms_expand_wild("", gcl);

    /* Verify the matches */
    ck_assert_ptr_nonnull(matches->matches);
    ck_assert_ptr_null(matches->matches[0]); // Should be empty list
}
END_TEST

/* Test 6: Pattern matches all files */
START_TEST(test_ms_expand_wild_all_files)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    ck_assert_int_eq(chdir(temp_dir), 0);
    printf("Current directory after chdir: %s\n", getcwd(NULL, 0)); // Debug statement

    /* Create files */
    const char *filenames[] = {"file1", "file2", "file3", NULL};
    create_files(temp_dir, filenames);

    /* Pattern to match all files */
    const char *pattern = "*";

    /* Call the function */
    t_wildcard_context *matches = ms_expand_wild(pattern, gcl);

    /* Verify the matches */
    ck_assert_ptr_nonnull(matches);
    /* Expecting 3 matches */
    ck_assert_ptr_nonnull(matches->matches[0]);
    ck_assert_ptr_nonnull(matches->matches[1]);
    ck_assert_ptr_nonnull(matches->matches[2]);
    ck_assert_ptr_null(matches->matches[3]);

    /* Check that matches are "file1", "file2", "file3" */
    bool match1 = false, match2 = false, match3 = false;
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(matches->matches[i], "file1") == 0)
            match1 = true;
        else if (strcmp(matches->matches[i], "file2") == 0)
            match2 = true;
        else if (strcmp(matches->matches[i], "file3") == 0)
            match3 = true;
    }
    ck_assert(match1);
    ck_assert(match2);
    ck_assert(match3);

    /* Clean up */
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 7: Files with special characters */
START_TEST(test_ms_expand_wild_special_characters)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    ck_assert_int_eq(chdir(temp_dir), 0);
    printf("Current directory after chdir: %s\n", getcwd(NULL, 0)); // Debug statement

    /* Create files with special characters */
    const char *filenames[] = {"file name.txt", "file-name.txt", "file_name.txt", NULL};
    create_files(temp_dir, filenames);

    /* Pattern to match files with space */
    const char *pattern = "file name.txt";

    /* Call the function */
    t_wildcard_context *matches = ms_expand_wild(pattern, gcl);

    /* Verify the matches */
    ck_assert_ptr_nonnull(matches->matches);
    ck_assert_ptr_nonnull(matches->matches[0]);
    ck_assert_ptr_null(matches->matches[1]);

    ck_assert_str_eq(matches->matches[0], "file name.txt");

    /* Clean up */
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 8: Case sensitivity */
START_TEST(test_ms_expand_wild_case_sensitivity)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    ck_assert_int_eq(chdir(temp_dir), 0);
    printf("Current directory after chdir: %s\n", getcwd(NULL, 0)); // Debug statement

    /* Create files */
    const char *filenames[] = {"Test.c", "test.c", "TEST.C", NULL};
    create_files(temp_dir, filenames);

    /* Pattern to match */
    const char *pattern = "test.c";

    /* Call the function */
    t_wildcard_context *matches = ms_expand_wild(pattern, gcl);

    /* Verify the matches */
    ck_assert_ptr_nonnull(matches->matches);
    ck_assert_ptr_nonnull(matches->matches[0]);
    ck_assert_ptr_null(matches->matches[1]);

    ck_assert_str_eq(matches->matches[0], "test.c");

    /* Clean up */
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 9: Complex patterns */
START_TEST(test_ms_expand_wild_complex_patterns)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    ck_assert_int_eq(chdir(temp_dir), 0);
    printf("Current directory after chdir: %s\n", getcwd(NULL, 0)); // Debug statement

    /* Create files */
    const char *filenames[] = {"abc.txt", "abcd.txt", "abcde.txt", "xyz.txt", NULL};
    create_files(temp_dir, filenames);

    /* Pattern to match */
    const char *pattern = "abc*.txt";

    /* Call the function */
    t_wildcard_context *matches = ms_expand_wild(pattern, gcl);

    /* Verify the matches */
    ck_assert_ptr_nonnull(matches->matches);
    /* Expecting 3 matches */
    ck_assert_ptr_nonnull(matches->matches[0]);
    ck_assert_ptr_nonnull(matches->matches[1]);
    ck_assert_ptr_nonnull(matches->matches[2]);
    ck_assert_ptr_null(matches->matches[3]);

    /* Check that matches are "abc.txt", "abcd.txt", "abcde.txt" */
    bool match1 = false, match2 = false, match3 = false;
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(matches->matches[i], "abc.txt") == 0)
            match1 = true;
        else if (strcmp(matches->matches[i], "abcd.txt") == 0)
            match2 = true;
        else if (strcmp(matches->matches[i], "abcde.txt") == 0)
            match3 = true;
    }
    ck_assert(match1);
    ck_assert(match2);
    ck_assert(match3);

    /* Clean up */
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test Suite Setup */
Suite *ms_expand_wild_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_expand_wild");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_expand_wild_simple_pattern);
    tcase_add_test(tc_core, test_ms_expand_wild_no_matches);
    tcase_add_test(tc_core, test_ms_expand_wild_hidden_files);
    tcase_add_test(tc_core, test_ms_expand_wild_empty_pattern);
    tcase_add_test(tc_core, test_ms_expand_wild_all_files);
    tcase_add_test(tc_core, test_ms_expand_wild_special_characters);
    tcase_add_test(tc_core, test_ms_expand_wild_case_sensitivity);
    tcase_add_test(tc_core, test_ms_expand_wild_complex_patterns);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_expand_wild_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
