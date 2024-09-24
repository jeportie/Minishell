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
    # Extract the name of the test (e.g., "test_ft_split" from "test_ft_split.c")
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    
    # Create a test executable
    add_executable(${TEST_NAME} ${TEST_FILE} ${SRC_FILES})

    # Link the Check and Readline libraries
    target_link_libraries(${TEST_NAME} ${CHECK_LIBRARIES} ${READLINE_LIBRARIES})
    target_include_directories(${TEST_NAME} PUBLIC ${CHECK_INCLUDE_DIRS})

    # Add the test to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    # Add custom CTest options (e.g., timeout) for each test
    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 5)
endforeach()
