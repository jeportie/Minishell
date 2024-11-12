/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_execute_external.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: assistant <assistant@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:00:00 by assistant         #+#    #+#             */
/*   Updated: 2024/11/12 12:58:24 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/exec.h"
#include "../../include/process.h"
#include "../../include/minishell.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

static t_gc *gcl;

/* Helper function to create a temporary executable file */
char *create_temp_executable(const char *script_content)
{
    char template[] = "/tmp/test_ms_execute_external_XXXXXX";
    int fd = mkstemp(template);
    if (fd == -1)
    {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }
    /* Write script content */
    write(fd, script_content, strlen(script_content));
    close(fd);
    /* Make the file executable */
    chmod(template, 0755);
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

/* Test 1: Execute a valid command */
START_TEST(test_ms_execute_external_valid_command)
{
    /* Create a temporary executable script */
    const char *script_content = "#!/bin/sh\necho Hello World\n";
    char *executable_path = create_temp_executable(script_content);

    /* Initialize command node */
    t_cmd_node cmd_node;
    char *argv[] = {executable_path, NULL};
    cmd_node.argv = argv;

    /* Initialize execution context */
    t_exec_context context;
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    env_data.env = NULL; /* Empty environment */
    env_data.path = "/bin:/usr/bin"; /* Include standard paths */
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_external(&cmd_node, &context, manager, gcl);

    /* Verify the result */
    ck_assert_int_eq(ret, 0); /* Expect exit status 0 */

    /* Clean up */
    remove_file(executable_path);
    free(executable_path);
    clean_manager(manager);
}
END_TEST

/* Test 2: Command not found */
START_TEST(test_ms_execute_external_command_not_found)
{
    /* Initialize command node */
    t_cmd_node cmd_node;
    char *argv[] = {"nonexistent_command", NULL};
    cmd_node.argv = argv;

    /* Initialize execution context */
    t_exec_context context;
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    env_data.env = NULL; /* Empty environment */
    env_data.path = "/bin:/usr/bin";
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Redirect stderr to capture error message */
    int stderr_pipe[2];
    ck_assert_int_eq(pipe(stderr_pipe), 0);
    int saved_stderr = dup(STDERR_FILENO);
    ck_assert_int_eq(dup2(stderr_pipe[1], STDERR_FILENO), STDERR_FILENO);
    close(stderr_pipe[1]);

    /* Call the function */
    int ret = ms_execute_external(&cmd_node, &context, manager, gcl);

    /* Restore stderr */
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    /* Read error message */
    char error_buffer[256];
    ssize_t bytes_read = read(stderr_pipe[0], error_buffer, sizeof(error_buffer) - 1);
    ck_assert_int_gt(bytes_read, 0);
    error_buffer[bytes_read] = '\0';
    close(stderr_pipe[0]);

    /* Verify the result */
    ck_assert_int_eq(ret, 127); /* Command not found exit status */
    ck_assert_str_eq(context.exit_status, 127);

    /* Check error message */
    ck_assert_msg(strstr(error_buffer, "minishell: command not found: nonexistent_command") != NULL,
                  "Error message not as expected");

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 3: Command execution failure (permission denied) */
START_TEST(test_ms_execute_external_permission_denied)
{
    /* Create a temporary file without execute permissions */
    const char *script_content = "#!/bin/sh\necho Hello World\n";
    char *executable_path = create_temp_executable(script_content);
    chmod(executable_path, 0644); /* Remove execute permissions */

    /* Initialize command node */
    t_cmd_node cmd_node;
    char *argv[] = {executable_path, NULL};
    cmd_node.argv = argv;

    /* Initialize execution context */
    t_exec_context context;
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    env_data.env = NULL;
    env_data.path = "/bin:/usr/bin";
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Call the function */
    int ret = ms_execute_external(&cmd_node, &context, manager, gcl);

    /* Verify the result */
    /* Since execve fails, the child process should exit with EXIT_FAILURE */
    ck_assert_int_eq(ret, EXIT_FAILURE);
    ck_assert_int_eq(context.exit_status, EXIT_FAILURE);

    /* Clean up */
    remove_file(executable_path);
    free(executable_path);
    clean_manager(manager);
}
END_TEST

/* Test 4: Command with arguments */
START_TEST(test_ms_execute_external_with_arguments)
{
    /* Create a temporary executable script that echoes arguments */
    const char *script_content = "#!/bin/sh\necho \"$@\"\n";
    char *executable_path = create_temp_executable(script_content);

    /* Initialize command node */
    t_cmd_node cmd_node;
    char *argv[] = {executable_path, "arg1", "arg2", NULL};
    cmd_node.argv = argv;

    /* Initialize execution context */
    t_exec_context context;
    context.child_lvl = 0;
    context.stdin_fd = STDIN_FILENO;
    context.stdout_fd = STDOUT_FILENO;
    context.stderr_fd = STDERR_FILENO;
    context.is_subprocess = false;
    context.exit_status = 0;
    t_shell shell;
    t_env_data env_data;
    shell.env_data = &env_data;
    shell.gcl = gcl;
    env_data.env = NULL;
    env_data.path = "/bin:/usr/bin";
    context.shell = &shell;

    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);

    /* Redirect stdout to capture output */
    int stdout_pipe[2];
    ck_assert_int_eq(pipe(stdout_pipe), 0);
    int saved_stdout = dup(STDOUT_FILENO);
    ck_assert_int_eq(dup2(stdout_pipe[1], STDOUT_FILENO), STDOUT_FILENO);
    close(stdout_pipe[1]);

    /* Call the function */
    int ret = ms_execute_external(&cmd_node, &context, manager, gcl);

    /* Restore stdout */
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    /* Read output */
    char output_buffer[256];
    ssize_t bytes_read = read(stdout_pipe[0], output_buffer, sizeof(output_buffer) - 1);
    ck_assert_int_gt(bytes_read, 0);
    output_buffer[bytes_read] = '\0';
    close(stdout_pipe[0]);

    /* Verify the result */
    ck_assert_int_eq(ret, 0);
    ck_assert_str_eq(context.exit_status, 0);

    /* Check output */
    /* Remove trailing newline */
    if (output_buffer[bytes_read - 1] == '\n')
        output_buffer[bytes_read - 1] = '\0';
    ck_assert_str_eq(output_buffer, "arg1 arg2");

    /* Clean up */
    remove_file(executable_path);
    free(executable_path);
    clean_manager(manager);
}
END_TEST

/* Additional tests can be added to cover more edge cases */

Suite *ms_execute_external_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_execute_external");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_ms_execute_external_valid_command);
    tcase_add_test(tc_core, test_ms_execute_external_command_not_found);
    tcase_add_test(tc_core, test_ms_execute_external_permission_denied);
    tcase_add_test(tc_core, test_ms_execute_external_with_arguments);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_execute_external_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

