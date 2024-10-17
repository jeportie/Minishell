/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ms_stop_std_signal.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:30:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/15 13:31:17 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <check.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Helper function to test signal handling */
int test_signal_handling(void (*init_signal_func)(void), int signal_to_send)
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        init_signal_func();
        pause(); // Wait for a signal
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0) // Parent process
    {
        sleep(1); // Give the child time to set up signal handling
        kill(pid, signal_to_send);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status); // Return the exit code
        }
        else if (WIFSIGNALED(status))
        {
            return WTERMSIG(status); // Return the signal that caused the termination
        }
    }
    return -1; // Fork failed
}

START_TEST(test_ms_stop_std_signal_sigint)
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        ms_stop_std_signal();
        raise(SIGINT); // Simulate SIGINT
        exit(EXIT_SUCCESS); // Should not be terminated by SIGINT
    }
    else if (pid > 0) // Parent process
    {
        int status;
        waitpid(pid, &status, 0);
        ck_assert(WIFEXITED(status)); // Check if the child exited normally
    }
}
END_TEST

START_TEST(test_ms_stop_std_signal_sigquit)
{
    pid_t pid = fork();
    if (pid == 0) // Child process
    {
        ms_stop_std_signal();
        raise(SIGQUIT); // Simulate SIGQUIT
        exit(EXIT_SUCCESS); // Should not be terminated by SIGQUIT
    }
    else if (pid > 0) // Parent process
    {
        int status;
        waitpid(pid, &status, 0);
        ck_assert(WIFEXITED(status)); // Check if the child exited normally
    }
}
END_TEST

Suite *ms_stop_std_signal_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("ms_stop_std_signal");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ms_stop_std_signal_sigint);
    tcase_add_test(tc_core, test_ms_stop_std_signal_sigquit);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = ms_stop_std_signal_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
