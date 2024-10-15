cmake_minimum_required(VERSION 3.10)
project(minishell_tests C)

# Enable testing with CTest
enable_testing()

# Option to enable or disable Valgrind
option(ENABLE_VALGRIND "Enable memory leak testing with Valgrind" ON)

# Set project directories relative to the test_src directory
set(ROOT_DIR "${CMAKE_SOURCE_DIR}/..")
set(SRC_DIR "${ROOT_DIR}/src")
set(INCLUDE_DIR "${ROOT_DIR}/include")
set(TEST_SRC_DIR "${CMAKE_SOURCE_DIR}")

# Set libgc and libft directories
set(LIBGC_INCLUDE_DIR "${ROOT_DIR}/lib/libgc/include")
set(LIBGC_LIB "${ROOT_DIR}/lib/libgc/libgc.a")
set(LIBFT_INCLUDE_DIR "${ROOT_DIR}/lib/libft/include")
set(LIBFT_LIB "${ROOT_DIR}/lib/libft/libft.a")

# Find Check and Readline libraries using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)
pkg_check_modules(READLINE REQUIRED readline)

# Include the header files, including libgc and libft
include_directories(
    ${INCLUDE_DIR}
    ${READLINE_INCLUDE_DIRS}
    ${LIBGC_INCLUDE_DIR}
    ${LIBFT_INCLUDE_DIR}
    ${CHECK_INCLUDE_DIRS}
)

# Automatically include all .c files from src/ and test_src/
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c")
file(GLOB_RECURSE TEST_SRC_FILES "${TEST_SRC_DIR}/*.c")

# Find Valgrind
find_program(VALGRIND_EXECUTABLE valgrind)

# Suppressions file (if you have one)
set(VALGRIND_SUPPRESSIONS_FILE "${ROOT_DIR}/assets/supp.supp")

# Add the test executable for each test source file
foreach(TEST_FILE ${TEST_SRC_FILES})
    # Extract the executable name (e.g., "test_ms_get_user_input" from "test_ms_get_user_input.c")
    get_filename_component(EXEC_NAME ${TEST_FILE} NAME_WE)

    # Remove the "test_" prefix from the test name for display purposes
    string(REGEX REPLACE "^test_" "" TEST_NAME ${EXEC_NAME})

    # Create a test executable with the original name
    add_executable(${EXEC_NAME} ${TEST_FILE} ${SRC_FILES})

    # Link the Check, Readline, libgc, and libft libraries
    target_link_libraries(
        ${EXEC_NAME}
        ${CHECK_LIBRARIES}
        ${READLINE_LIBRARIES}
        ${LIBGC_LIB}
        ${LIBFT_LIB}
    )

    # Add include directories
    target_include_directories(
        ${EXEC_NAME} PUBLIC
        ${CHECK_INCLUDE_DIRS}
        ${LIBGC_INCLUDE_DIR}
        ${LIBFT_INCLUDE_DIR}
        ${INCLUDE_DIR}
    )

    # Add the test to CTest with the test name without the prefix
    if(ENABLE_VALGRIND AND VALGRIND_EXECUTABLE)
        # Run the test under Valgrind
        add_test(
            NAME ${TEST_NAME}
            COMMAND ${VALGRIND_EXECUTABLE}
            --leak-check=full
            --error-exitcode=1
            --quiet
            --suppressions=${VALGRIND_SUPPRESSIONS_FILE}
            $<TARGET_FILE:${EXEC_NAME}>
        )
        # Optionally, set properties to fail the test if memory leaks are found
        # set_tests_properties(${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION "definitely lost: [1-9][0-9]* bytes")
    else()
        # Run the test normally
        add_test(NAME ${TEST_NAME} COMMAND $<TARGET_FILE:${EXEC_NAME}>)
    endif()

    # Add custom CTest options (e.g., timeout) for each test
    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 5)
endforeach()

