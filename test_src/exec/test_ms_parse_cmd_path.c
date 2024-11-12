/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_parse_cmd_path.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: assistant <assistant@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:00:00 by assistant         #+#    #+#             */
/*   Updated: 2024/11/12 12:42:14 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/exec.h"
#include "../../include/minishell.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

static t_gc *gcl;

/* Helper function to create a temporary directory */
char *create_temp_dir(void)
{
    char template[] = "/tmp/test_ms_parse_cmd_path_XXXXXX";
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

/* Helper function to create an executable file */
void create_executable_file(const char *dir, const char *filename)
{
    char filepath[PATH_MAX];
    snprintf(filepath, sizeof(filepath), "%s/%s", dir, filename);
    int fd = open(filepath, O_CREAT | O_WRONLY, 0755);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    /* Write a simple script or binary content */
    write(fd, "#!/bin/sh\necho Hello World\n", 26);
    close(fd);
}

/* Helper function to create a non-executable file */
void create_non_executable_file(const char *dir, const char *filename)
{
    char filepath[PATH_MAX];
    snprintf(filepath, sizeof(filepath), "%s/%s", dir, filename);
    int fd = open(filepath, O_CREAT | O_WRONLY, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    write(fd, "This is not executable.\n", 24);
    close(fd);
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

/* Test 1: Command with absolute path */
START_TEST(test_ms_parse_cmd_path_absolute)
{
    char *temp_dir = create_temp_dir();

    /* Create an executable file */
    create_executable_file(temp_dir, "my_command");

    /* Build absolute path */
    char abs_path[PATH_MAX];
    snprintf(abs_path, sizeof(abs_path), "%s/my_command", temp_dir);

    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Call the function */
    char *result = ms_parse_cmd_path(abs_path, &shell);

    /* Verify the result */
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, abs_path);

    /* Clean up */
    free(result);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 2: Command with relative path */
START_TEST(test_ms_parse_cmd_path_relative)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    /* Change to the temporary directory */
    ck_assert_int_eq(chdir(temp_dir), 0);

    /* Create an executable file */
    create_executable_file(".", "my_command");

    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Command is './my_command' */
    char *command = "./my_command";

    /* Call the function */
    char *result = ms_parse_cmd_path(command, &shell);

    /* Verify the result */
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, command);

    /* Clean up */
    free(result);
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 3: Command not found in PATH */
START_TEST(test_ms_parse_cmd_path_not_found)
{
    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Set PATH to some directory without the command */
    env_data.path = "/usr/bin:/bin";

    /* Command is 'nonexistent_command' */
    char *command = "nonexistent_command";

    /* Call the function */
    char *result = ms_parse_cmd_path(command, &shell);

    /* Verify the result */
    ck_assert_ptr_null(result);
}
END_TEST

/* Test 4: Command found in PATH */
START_TEST(test_ms_parse_cmd_path_found_in_path)
{
    char *temp_dir = create_temp_dir();

    /* Create an executable file */
    create_executable_file(temp_dir, "my_command");

    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Set PATH to include the temp_dir */
    env_data.path = temp_dir;

    /* Command is 'my_command' */
    char *command = "my_command";

    /* Call the function */
    char *result = ms_parse_cmd_path(command, &shell);

    /* Verify the result */
    ck_assert_ptr_nonnull(result);

    /* Build expected path */
    char expected_path[PATH_MAX];
    snprintf(expected_path, sizeof(expected_path), "%s/my_command", temp_dir);

    ck_assert_str_eq(result, expected_path);

    /* Clean up */
    free(result);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 5: Command with slash but non-executable */
START_TEST(test_ms_parse_cmd_path_non_executable)
{
    char *temp_dir = create_temp_dir();

    /* Create a non-executable file */
    create_non_executable_file(temp_dir, "my_command");

    /* Build absolute path */
    char abs_path[PATH_MAX];
    snprintf(abs_path, sizeof(abs_path), "%s/my_command", temp_dir);

    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Call the function */
    char *result = ms_parse_cmd_path(abs_path, &shell);

    /* Verify the result */
    ck_assert_ptr_null(result);

    /* Clean up */
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 6: Empty PATH variable */
START_TEST(test_ms_parse_cmd_path_empty_path)
{
    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Set PATH to empty string */
    env_data.path = "";

    /* Command is 'ls' */
    char *command = "ls";

    /* Call the function */
    char *result = ms_parse_cmd_path(command, &shell);

    /* Verify the result */
    ck_assert_ptr_null(result);
}
END_TEST

/* Test 7: PATH variable with empty entries */
START_TEST(test_ms_parse_cmd_path_empty_entries_in_path)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    /* Change to the temporary directory */
    ck_assert_int_eq(chdir(temp_dir), 0);

    /* Create an executable file in current directory */
    create_executable_file(".", "my_command");

    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Set PATH with empty entries */
    env_data.path = "::";

    /* Command is 'my_command' */
    char *command = "my_command";

    /* Call the function */
    char *result = ms_parse_cmd_path(command, &shell);

    /* Verify the result */
    ck_assert_ptr_nonnull(result);

    /* Expected path is './my_command' */
    char expected_path[PATH_MAX];
    snprintf(expected_path, sizeof(expected_path), "./my_command");

    ck_assert_str_eq(result, expected_path);

    /* Clean up */
    free(result);
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 8: PATH variable starts and ends with ':' */
START_TEST(test_ms_parse_cmd_path_colon_path)
{
    char *temp_dir = create_temp_dir();
    char *original_dir = getcwd(NULL, 0);
    ck_assert_ptr_nonnull(original_dir);

    /* Change to the temporary directory */
    ck_assert_int_eq(chdir(temp_dir), 0);

    /* Create an executable file in current directory */
    create_executable_file(".", "my_command");

    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Set PATH with leading and trailing colons */
    env_data.path = ":/usr/bin:";

    /* Command is 'my_command' */
    char *command = "my_command";

    /* Call the function */
    char *result = ms_parse_cmd_path(command, &shell);

    /* Verify the result */
    ck_assert_ptr_nonnull(result);

    /* Expected path is './my_command' */
    char expected_path[PATH_MAX];
    snprintf(expected_path, sizeof(expected_path), "./my_command");

    ck_assert_str_eq(result, expected_path);

    /* Clean up */
    free(result);
    ck_assert_int_eq(chdir(original_dir), 0);
    free(original_dir);
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 9: Command with special characters */
START_TEST(test_ms_parse_cmd_path_special_characters)
{
    char *temp_dir = create_temp_dir();

    /* Create an executable file */
    create_executable_file(temp_dir, "my command");

    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Set PATH to include the temp_dir */
    env_data.path = temp_dir;

    /* Command is 'my command' */
    char *command = "my command";

    /* Call the function */
    char *result = ms_parse_cmd_path(command, &shell);

    /* Verify the result */
    ck_assert_ptr_null(result); /* Since command has spaces, it's unlikely to be found */

    /* Clean up */
    remove_directory(temp_dir);
    free(temp_dir);
}
END_TEST

/* Test 10: Non-existent directories in PATH */
START_TEST(test_ms_parse_cmd_path_invalid_directories)
{
    /* Initialize shell and env_data */
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;

    /* Set PATH with invalid directories */
    env_data.path = "/nonexistent:/alsonotreal";

    /* Command is 'ls' */
    char *command = "ls";

    /* Call the function */
    char *result = ms_parse_cmd_path(command, &shell);

    /* Verify the result */
    ck_assert_ptr_null(result); /* Should not find 'ls' in invalid directories */
}
END_TEST

/* Test Suite Setup */
Suite *ms_parse_cmd_path_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_parse_cmd_path");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_parse_cmd_path_absolute);
    tcase_add_test(tc_core, test_ms_parse_cmd_path_relative);
    tcase_add_test(tc_core, test_ms_parse_cmd_path_not_found);
    tcase_add_test(tc_core, test_ms_parse_cmd_path_found_in_path);
    tcase_add_test(tc_core, test_ms_parse_cmd_path_non_executable);
    tcase_add_test(tc_core, test_ms_parse_cmd_path_empty_path);
    tcase_add_test(tc_core, test_ms_parse_cmd_path_empty_entries_in_path);
    tcase_add_test(tc_core, test_ms_parse_cmd_path_colon_path);
    tcase_add_test(tc_core, test_ms_parse_cmd_path_special_characters);
    tcase_add_test(tc_core, test_ms_parse_cmd_path_invalid_directories);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_parse_cmd_path_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
