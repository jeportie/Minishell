cmake_minimum_required(VERSION 3.10)
project(minishell_tests C)

# Enable testing with CTest
enable_testing()

# Set project directories relative to the test_src directory
set(SRC_DIR "${CMAKE_SOURCE_DIR}/../src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../include")
set(TEST_SRC_DIR "${CMAKE_SOURCE_DIR}")

# Find Check and Readline libraries using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)
pkg_check_modules(READLINE REQUIRED readline)

# Include the header files
include_directories(${INCLUDE_DIR} ${READLINE_INCLUDE_DIRS})

# Automatically include all .c files from src/ and test_src/
file(GLOB SRC_FILES "${SRC_DIR}/**/*.c")
file(GLOB TEST_SRC_FILES "${TEST_SRC_DIR}/**/*.c")

# Add the test executable for each test source file
foreach(TEST_FILE ${TEST_SRC_FILES})
    # Extract the executable name (e.g., "test_ms_get_user_input" from "test_ms_get_user_input.c")
    get_filename_component(EXEC_NAME ${TEST_FILE} NAME_WE)

    # Remove the "test_" prefix from the test name for display purposes
    string(REGEX REPLACE "^test_" "" TEST_NAME ${EXEC_NAME})

    # Create a test executable with the original name
    add_executable(${EXEC_NAME} ${TEST_FILE} ${SRC_FILES})

    # Link the Check and Readline libraries
    target_link_libraries(${EXEC_NAME} ${CHECK_LIBRARIES} ${READLINE_LIBRARIES})
    target_include_directories(${EXEC_NAME} PUBLIC ${CHECK_INCLUDE_DIRS})

    # Add the test to CTest with the test name without the prefix, but command is the executable with prefix
    add_test(NAME ${TEST_NAME} COMMAND ${EXEC_NAME})

    # Add custom CTest options (e.g., timeout) for each test
    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 5)
endforeach()
