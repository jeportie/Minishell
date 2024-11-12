/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_handle_redirections.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: assistant <assistant@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 10:00:00 by assistant         #+#    #+#             */
/*   Updated: 2024/11/12 12:54:07 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/exec.h"
#include "../../include/process.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

static t_gc *gcl;

/* Helper function to create a temporary file */
char *create_temp_file(const char *content)
{
    char template[] = "/tmp/test_ms_handle_redirections_XXXXXX";
    int fd = mkstemp(template);
    if (fd == -1)
    {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }
    if (content)
    {
        write(fd, content, strlen(content));
        lseek(fd, 0, SEEK_SET);
    }
    close(fd);
    return strdup(template);
}

/* Helper function to remove a file */
void remove_file(const char *filepath)
{
    unlink(filepath);
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

/* Test 1: Input redirection from an existing file */
START_TEST(test_ms_handle_redirections_input_redirection)
{
    /* Create a temporary input file */
    char *input_file = create_temp_file("Test input content.\n");

    /* Initialize execution context */
    t_exec_context context;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Create AST node for input redirection */
    t_ast_node node;
    node.type = 2; /* Input redirection type */
    node.data.redirect.filename = input_file;
    node.data.redirect.child = NULL;

    /* Call the function */
    int ret = ms_handle_redirections(&node, &context, manager, gcl);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_ne(context.stdin_fd, STDIN_FILENO);

    /* Read from the new stdin_fd */
    char buffer[1024];
    ssize_t bytes_read = read(context.stdin_fd, buffer, sizeof(buffer) - 1);
    ck_assert_int_gt(bytes_read, 0);
    buffer[bytes_read] = '\0';

    /* Verify the content */
    ck_assert_str_eq(buffer, "Test input content.\n");

    /* Clean up */
    close(context.stdin_fd);
    remove_file(input_file);
    free(input_file);
    clean_manager(manager);
}
END_TEST

/* Test 2: Output redirection to a new file */
START_TEST(test_ms_handle_redirections_output_redirection)
{
    /* Create a temporary output file */
    char *output_file = create_temp_file(NULL);

    /* Initialize execution context */
    t_exec_context context;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Create AST node for output redirection */
    t_ast_node node;
    node.type = 3; /* Output redirection type */
    node.data.redirect.filename = output_file;
    node.data.redirect.child = NULL;

    /* Call the function */
    int ret = ms_handle_redirections(&node, &context, manager, gcl);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_ne(context.stdout_fd, STDOUT_FILENO);

    /* Write to the new stdout_fd */
    const char *output_content = "Test output content.\n";
    ssize_t bytes_written = write(context.stdout_fd, output_content, strlen(output_content));
    ck_assert_int_eq(bytes_written, (ssize_t)strlen(output_content));

    /* Read from the file to verify */
    int fd = open(output_file, O_RDONLY);
    ck_assert_int_ne(fd, -1);
    char buffer[1024];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    ck_assert_int_gt(bytes_read, 0);
    buffer[bytes_read] = '\0';
    close(fd);

    /* Verify the content */
    ck_assert_str_eq(buffer, "Test output content.\n");

    /* Clean up */
    close(context.stdout_fd);
    remove_file(output_file);
    free(output_file);
    clean_manager(manager);
}
END_TEST

/* Test 3: Output append redirection to an existing file */
START_TEST(test_ms_handle_redirections_output_append_redirection)
{
    /* Create a temporary output file with initial content */
    char *output_file = create_temp_file("Initial content.\n");

    /* Initialize execution context */
    t_exec_context context;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Create AST node for output append redirection */
    t_ast_node node;
    node.type = 4; /* Output append redirection type */
    node.data.redirect.filename = output_file;
    node.data.redirect.child = NULL;

    /* Call the function */
    int ret = ms_handle_redirections(&node, &context, manager, gcl);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_ne(context.stdout_fd, STDOUT_FILENO);

    /* Write to the new stdout_fd */
    const char *additional_content = "Appended content.\n";
    ssize_t bytes_written = write(context.stdout_fd, additional_content, strlen(additional_content));
    ck_assert_int_eq(bytes_written, (ssize_t)strlen(additional_content));

    /* Read from the file to verify */
    int fd = open(output_file, O_RDONLY);
    ck_assert_int_ne(fd, -1);
    char buffer[1024];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    ck_assert_int_gt(bytes_read, 0);
    buffer[bytes_read] = '\0';
    close(fd);

    /* Verify the content */
    const char *expected_content = "Initial content.\nAppended content.\n";
    ck_assert_str_eq(buffer, expected_content);

    /* Clean up */
    close(context.stdout_fd);
    remove_file(output_file);
    free(output_file);
    clean_manager(manager);
}
END_TEST

/* Test 4: Input redirection from a non-existent file */
START_TEST(test_ms_handle_redirections_input_nonexistent_file)
{
    /* Non-existent input file */
    char *input_file = "/nonexistent/file.txt";

    /* Initialize execution context */
    t_exec_context context;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Create AST node for input redirection */
    t_ast_node node;
    node.type = 2; /* Input redirection type */
    node.data.redirect.filename = input_file;
    node.data.redirect.child = NULL;

    /* Call the function */
    int ret = ms_handle_redirections(&node, &context, manager, gcl);

    /* Verify the result */
    ck_assert_int_eq(ret, -1);
    ck_assert_int_eq(context.stdin_fd, STDIN_FILENO);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 5: Output redirection with no write permission */
START_TEST(test_ms_handle_redirections_output_no_permission)
{
    /* Create a temporary output file */
    char *output_file = create_temp_file(NULL);

    /* Remove write permissions */
    chmod(output_file, 0444); /* Read-only */

    /* Initialize execution context */
    t_exec_context context;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Create AST node for output redirection */
    t_ast_node node;
    node.type = 3; /* Output redirection type */
    node.data.redirect.filename = output_file;
    node.data.redirect.child = NULL;

    /* Call the function */
    int ret = ms_handle_redirections(&node, &context, manager, gcl);

    /* Verify the result */
    ck_assert_int_eq(ret, -1);
    ck_assert_int_eq(context.stdout_fd, STDOUT_FILENO);

    /* Clean up */
    chmod(output_file, 0644); /* Restore permissions to allow deletion */
    remove_file(output_file);
    free(output_file);
    clean_manager(manager);
}
END_TEST

/* Test 6: Empty filename in redirection */
START_TEST(test_ms_handle_redirections_empty_filename)
{
    /* Initialize execution context */
    t_exec_context context;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Create AST node with empty filename */
    t_ast_node node;
    node.type = 2; /* Input redirection type */
    node.data.redirect.filename = "";
    node.data.redirect.child = NULL;

    /* Call the function */
    int ret = ms_handle_redirections(&node, &context, manager, gcl);

    /* Verify the result */
    ck_assert_int_eq(ret, -1);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 7: Unsupported redirection type */
START_TEST(test_ms_handle_redirections_invalid_type)
{
    /* Initialize execution context */
    t_exec_context context;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Create AST node with invalid type */
    t_ast_node node;
    node.type = 99; /* Invalid type */
    node.data.redirect.child = NULL;

    /* Call the function */
    int ret = ms_handle_redirections(&node, &context, manager, gcl);

    /* Verify the result */
    ck_assert_int_eq(ret, -1);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 8: Chained redirections (input and output) */
START_TEST(test_ms_handle_redirections_chained)
{
    /* Create temporary files */
    char *input_file = create_temp_file("Chained input content.\n");
    char *output_file = create_temp_file(NULL);

    /* Initialize execution context */
    t_exec_context context;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Create AST nodes for redirections */
    t_ast_node output_node;
    output_node.type = 3; /* Output redirection */
    output_node.data.redirect.filename = output_file;
    output_node.data.redirect.child = NULL;

    t_ast_node input_node;
    input_node.type = 2; /* Input redirection */
    input_node.data.redirect.filename = input_file;
    input_node.data.redirect.child = &output_node;

    /* Call the function */
    int ret = ms_handle_redirections(&input_node, &context, manager, gcl);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_int_ne(context.stdin_fd, STDIN_FILENO);
    ck_assert_int_ne(context.stdout_fd, STDOUT_FILENO);

    /* Read from the new stdin_fd */
    char buffer[1024];
    ssize_t bytes_read = read(context.stdin_fd, buffer, sizeof(buffer) - 1);
    ck_assert_int_gt(bytes_read, 0);
    buffer[bytes_read] = '\0';
    ck_assert_str_eq(buffer, "Chained input content.\n");

    /* Write to the new stdout_fd */
    const char *output_content = "Chained output content.\n";
    ssize_t bytes_written = write(context.stdout_fd, output_content, strlen(output_content));
    ck_assert_int_eq(bytes_written, (ssize_t)strlen(output_content));

    /* Read from the output file */
    int fd = open(output_file, O_RDONLY);
    ck_assert_int_ne(fd, -1);
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    ck_assert_int_gt(bytes_read, 0);
    buffer[bytes_read] = '\0';
    close(fd);
    ck_assert_str_eq(buffer, "Chained output content.\n");

    /* Clean up */
    close(context.stdin_fd);
    close(context.stdout_fd);
    remove_file(input_file);
    remove_file(output_file);
    free(input_file);
    free(output_file);
    clean_manager(manager);
}
END_TEST

/* Additional tests can be added to cover heredoc and other edge cases */

/* Test Suite Setup */
Suite *ms_handle_redirections_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_handle_redirections");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_handle_redirections_input_redirection);
    tcase_add_test(tc_core, test_ms_handle_redirections_output_redirection);
    tcase_add_test(tc_core, test_ms_handle_redirections_output_append_redirection);
    tcase_add_test(tc_core, test_ms_handle_redirections_input_nonexistent_file);
    tcase_add_test(tc_core, test_ms_handle_redirections_output_no_permission);
    tcase_add_test(tc_core, test_ms_handle_redirections_empty_filename);
    tcase_add_test(tc_core, test_ms_handle_redirections_invalid_type);
    tcase_add_test(tc_core, test_ms_handle_redirections_chained);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_handle_redirections_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
