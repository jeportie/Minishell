/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_testunit_template.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 09:57:55 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/21 11:20:02 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <check.h>
#include "../lib/libgc/include/libgc.h"

// Static global variables for the garbage collector or other shared resources.
// TIP: Keep globals to a minimum and avoid thread-unsafe shared data.
static t_gc *gcl;

/**
 * Setup function to initialize resources before each test case.
 * TIP: Always verify that initialization is successful.
 */
void setup(void)
{
    gcl = gc_init();
    if (!gcl)
    {
        perror("gc_init");
        exit(EXIT_FAILURE);
    }
}

/**
 * Teardown function to clean up resources after each test case.
 * TIP: Ensure all resources initialized in `setup` are properly cleaned here.
 */
void teardown(void)
{
    gc_cleanup(gcl);
    free(gcl);
}

/**
 * Example test case function.
 * TIP: Use meaningful names for your test functions to describe their purpose.
 */
START_TEST(name_of_the_test_function)
{
    // INIT FUNCTIONS and ENV
    // TIP: Use mock data or inputs relevant to the test.
    
    // CALL THE TESTED FUNCTION WITH PREDEFINED VALUES
    // TIP: Avoid side effects or dependencies on external systems (e.g., files, network).

    // USE THE ASSERT LIB TO COMPARE THE RESULTS WITH THE EXPECTED VALUES
    ck_assert_int_eq(1, 1); // Replace with actual assertions
}
END_TEST

/**
 * Suite setup to group related test cases.
 * TIP: Organize test cases by logical functionality (e.g., core, advanced features).
 */
Suite *name_suite(void)
{
    Suite *s;         // Test suite
    TCase *tc_core;   // Test case container

    s = suite_create("NAME"); // TIP: Use a descriptive name for the suite.

    tc_core = tcase_create("Core"); // TIP: Group related test functions logically.

    // Adding setup and teardown for this test case
    tcase_add_checked_fixture(tc_core, setup, teardown);

    // Adding individual test functions
    tcase_add_test(tc_core, name_of_the_test_function);

    // TIP: Add more test cases here as needed.
    suite_add_tcase(s, tc_core);

    return s;
}

/**
 * Main function to run the test suite.
 * TIP: Use different verbosity levels (CK_NORMAL, CK_VERBOSE) for debugging or CI environments.
 */
int main(void)
{
    int number_failed;   // Track number of failed tests
    Suite *name;         // Suite object
    SRunner *name_r;     // Test suite runner

    name = name_suite();
    name_r = srunner_create(name);

    // TIP: Use CK_VERBOSE for detailed output during development.
    srunner_run_all(name_r, CK_NORMAL); 

    number_failed = srunner_ntests_failed(name_r);
    srunner_free(name_r);

    // TIP: Integrate this with CI/CD pipelines to detect failures automatically.
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* 
 * ####################### USEFUL TIPS #######################
 * 
 * 1. **Modularize Your Tests**:
 *    - Divide test cases logically into suites and test functions.
 *    - Each test should focus on a single functionality or edge case.
 * 
 * 2. **Use Fixtures for Reusability**:
 *    - Use `setup` and `teardown` for initializing and cleaning up shared resources.
 * 
 * 3. **Custom Assertions**:
 *    - Implement helper functions for complex custom assertions to simplify test logic.
 * 
 * 4. **CI/CD Integration**:
 *    - Ensure the test suite is compatible with automated pipelines.
 *    - Use `CK_VERBOSE` for debugging failures in CI environments.
 * 
 * 5. **Memory Management**:
 *    - Always clean up resources in `teardown` to avoid memory leaks during repeated test runs.
 * 
 * 6. **Test Edge Cases**:
 *    - Include edge cases, invalid inputs, and extreme values in your test cases.
 * 
 * 7. **Error Logs**:
 *    - Use `srunner_set_log()` to log test results for debugging.
 * 
 * 8. **Tags for Selective Runs**:
 *    - Use `tcase_set_tags()` and `srunner_run_tagged()` for large projects to selectively run tests.
 * 
 * 9. **Debugging Failures**:
 *    - For persistent test failures, use debugging tools like `gdb` with verbose output.
 * 
 * 10. **Code Coverage**:
 *     - Ensure your tests cover all branches and conditions in your code.
 * 
 * ######################## CHECK FUNCTIONS ########################

## ck_assert functions in the Check framework:
|-------------------------------|---------------------------------------------|
| **Function**                  | **Description -> Check if ...**             |
|-------------------------------|---------------------------------------------|
|ck_assert(expr)                |the expression expr is true (non-zero).      |
|ck_assert_msg(expr, ...)       |expr is true, with message on failure.       |
|ck_assert_int_eq(val1, val2)   |two integers are equal.                      |
|ck_assert_int_ne(val1, val2)   |two integers are not equal.                  |
|ck_assert_int_lt(val1, val2)   |val1 is less than  al2.                      |
|ck_assert_int_le(val1, val2)   |val1 is less than or equal to `val2`.        |
|ck_assert_int_gt(val1, val2)   |val1 is greater than `val2`.                 |
|ck_assert_int_ge(val1, val2)   |val1 is greater than or equal to `val2`.     |
|ck_assert_float_eq(val1, val2) |two floats are equal.                        |
|ck_assert_float_ne(val1, val2) |two floats are not equal.                    |
|ck_assert_float_lt(val1, val2) |val1 is less than `val2`.                    |
|ck_assert_float_le(val1, val2) |val1 is less than or equal to `val2`.        |
|ck_assert_float_gt(val1, val2) |val1 is greater than val2.                   |
|ck_assert_float_ge(val1, val2) |val1 is greater than or equal to val2.       |
|ck_assert_ptr_eq(ptr1, ptr2)   |two pointers are equal.                      |
|ck_assert_ptr_ne(ptr1, ptr2)   |two pointers are not equal.                  |
|ck_assert_str_eq(str1, str2)   |two strings are equal.                       |
|ck_assert_str_ne(str1, str2)   |two strings are not equal.                   |
|ck_assert_str_lt(str1, str2)   |str1 is less than str2.                      |
|ck_assert_str_le(str1, str2)   |str1 is less than or equal to str2.          |
|ck_assert_str_gt(str1, str2)   |str1 is greater than str2.                   |
|ck_assert_str_ge(str1, str2)   |str1 is greater than or equal to str2.       |
|ck_assert_mem_eq(ptr1, ptr2, n)|two memory regions are equal for n bytes.    |
|ck_assert_mem_ne(ptr1, ptr2, n)|two memory regions are not equal for n bytes.|
|-------------------------------|---------------------------------------------|

### Suite Management

|-----------------------------------------------------|----------------------------------------------|
| **Function**                                        | **Description**                              |
|-----------------------------------------------------|----------------------------------------------|
|Suite *suite_create(const char *name)                | Creates a new test suite with the given name.|
|void suite_add_tcase(Suite *suite, TCase *tcase)     | Adds a test case to a test suite.            |
|Suite *suite_create_full(const char *name, TCase *tc)| Creates a full suite with a single test case.|
|-----------------------------------------------------|----------------------------------------------|

### Test Case Management

|-----------------------------------------------------|---------------------------------------------|
| **Function**                                        | **Description**                             |
|-----------------------------------------------------|---------------------------------------------|
|TCase *tcase_create(const char *name)                | Creates a new test case with the given name.|
|void tcase_add_test(TCase *tc, TFun test_function)   | Adds a test function to a test case.        |
|void tcase_add_loop_test(TCase *tc,                  | Adds a looping test to a test case,         |
|           TFun test_function, int start, int end)   | running the function multiple times.        |
|void tcase_set_timeout(TCase *tc, double timeout)    | Sets a timeout for a test case.             |
|void tcase_add_checked_fixture(TCase *tc,            | Adds setup and teardown functions.          |
|void (*setup)(void), void (*teardown)(void))         | Adds setup and teardown functions.          |
|-----------------------------------------------------|---------------------------------------------|

### Test Runner Management

|--------------------------------------------------------------|-----------------------------------------------------------|
| **Function**                                                 | **Description**                                           |
|--------------------------------------------------------------|-----------------------------------------------------------|
|SRunner *srunner_create(Suite *suite)                         | Creates a new test suite runner.                          |
|void srunner_add_suite(SRunner *runner, Suite *suite)         | Adds a suite to a suite runner.                           |
|void srunner_run_all(SRunner *runner, enum print_output)      | Runs all test cases in all suites managed by the runner.  |
|void srunner_run_tagged(SRunner *runner,                      | Runs only tests tagged with a specific label.             |
|         enum print_output, const char *tag)                  | Runs only tests tagged with a specific label.             |
|int srunner_ntests_failed(SRunner *runner)                    | Returns the number of tests that failed.                  |
|void srunner_free(SRunner *runner)                            | Frees the memory allocated to the suite runner.           |
|void srunner_set_log(SRunner *runner, const char *logfile)    | Specifies a file to log test results.                     |
|void srunner_set_tap(SRunner *runner, const char *tapfile)    | Specifies a file for TAP (Test Anything Protocol) results.|
|void srunner_set_fork_status(SRunner *runner, int fork_status)| Sets forking mode: `CK_FORK` or `CK_NOFORK`.              |
|void srunner_set_xml(SRunner *runner, const char *xmlfile)    | Specifies a file to output test results in XML format.    |
|void srunner_print(SRunner *runner,                           | Prints test results directly to the console               |
|        enum print_output)                                    | with the specified verbosity.                             |
|--------------------------------------------------------------|-----------------------------------------------------------|
*/
