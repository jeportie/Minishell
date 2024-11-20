/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_add_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:39:02 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 13:39:06 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../../include/process.h"
#include "../../lib/libgc/include/libgc.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static t_gc *gcl;

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

/* Test 1: Valid inputs */
START_TEST(test_add_process_valid_inputs)
{
    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);
    ck_assert_ptr_nonnull(manager);
    ck_assert_int_eq(manager->process_count, 0);
    ck_assert_ptr_null(manager->head);

    /* Set up process parameters */
    t_proc_params params;
    params.pid = 12345;
    params.parent_pid = 54321;
    params.child_lvl = 1;
    params.fd_in = 0;
    params.fd_out = 1;
    params.fd_error = 2;
    params.is_heredoc = false;
    params.title = "Test Process";

    /* Call the function */
    t_proc_info *proc_info = add_process(manager, &params);

    /* Verify the result */
    ck_assert_ptr_nonnull(proc_info);
    ck_assert_int_eq(manager->process_count, 1);
    ck_assert_ptr_eq(manager->head, proc_info);

    /* Verify process fields */
    ck_assert_int_eq(proc_info->pid, 12345);
    ck_assert_int_eq(proc_info->parent_pid, 54321);
    ck_assert_int_eq(proc_info->child_lvl, 1);
    ck_assert_int_eq(proc_info->fd_in, 0);
    ck_assert_int_eq(proc_info->fd_out, 1);
    ck_assert_int_eq(proc_info->fd_error, 2);
    ck_assert_int_eq(proc_info->fd_original_stdout, -1);
    ck_assert(proc_info->is_heredoc == false);
    ck_assert_str_eq(proc_info->title, "Test Process");
    ck_assert_ptr_null(proc_info->next);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 2: manager is NULL */
START_TEST(test_add_process_manager_null)
{
    /* Set up process parameters */
    t_proc_params params;
    params.pid = 12345;
    params.parent_pid = 54321;
    params.child_lvl = 1;
    params.fd_in = 0;
    params.fd_out = 1;
    params.fd_error = 2;
    params.is_heredoc = false;
    params.title = "Test Process";

    /* Call the function */
    t_proc_info *proc_info = add_process(NULL, &params);

    /* Verify the result */
    ck_assert_ptr_null(proc_info);
}
END_TEST

/* Test 3: params is NULL */
START_TEST(test_add_process_params_null)
{
    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);
    ck_assert_ptr_nonnull(manager);

    /* Call the function */
    t_proc_info *proc_info = add_process(manager, NULL);

    /* Verify the result */
    ck_assert_ptr_null(proc_info);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test 4: Adding multiple processes */
START_TEST(test_add_process_multiple)
{
    /* Initialize process manager */
    t_proc_manager *manager = init_manager(gcl);
    ck_assert_ptr_nonnull(manager);
    ck_assert_int_eq(manager->process_count, 0);
    ck_assert_ptr_null(manager->head);

    /* Set up process parameters for multiple processes */
    t_proc_params params1 = {12345, 54321, 1, 0, 1, 2, false, "Process 1"};
    t_proc_params params2 = {23456, 65432, 2, 3, 4, 5, true, "Process 2"};
    t_proc_params params3 = {34567, 76543, 3, 6, 7, 8, false, "Process 3"};

    /* Add first process */
    t_proc_info *proc_info1 = add_process(manager, &params1);
    ck_assert_ptr_nonnull(proc_info1);
    ck_assert_int_eq(manager->process_count, 1);
    ck_assert_ptr_eq(manager->head, proc_info1);

    /* Add second process */
    t_proc_info *proc_info2 = add_process(manager, &params2);
    ck_assert_ptr_nonnull(proc_info2);
    ck_assert_int_eq(manager->process_count, 2);
    ck_assert_ptr_eq(manager->head, proc_info2);

    /* Add third process */
    t_proc_info *proc_info3 = add_process(manager, &params3);
    ck_assert_ptr_nonnull(proc_info3);
    ck_assert_int_eq(manager->process_count, 3);
    ck_assert_ptr_eq(manager->head, proc_info3);

    /* Verify linked list */
    ck_assert_ptr_eq(proc_info3->next, proc_info2);
    ck_assert_ptr_eq(proc_info2->next, proc_info1);
    ck_assert_ptr_null(proc_info1->next);

    /* Clean up */
    clean_manager(manager);
}
END_TEST

/* Test Suite Setup */
Suite *add_process_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("add_process");

    /* Core test case */
    tc_core = tcase_create("Core");

    /* Setup and Teardown */
    tcase_add_checked_fixture(tc_core, setup, teardown);

    /* Add Tests */
    tcase_add_test(tc_core, test_add_process_valid_inputs);
    tcase_add_test(tc_core, test_add_process_manager_null);
    tcase_add_test(tc_core, test_add_process_params_null);
    tcase_add_test(tc_core, test_add_process_multiple);

    suite_add_tcase(s, tc_core);

    return s;
}

/* Main function to run the test suite */
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = add_process_suite();
    sr = srunner_create(s);

    /* You can change the verbosity level here */
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
