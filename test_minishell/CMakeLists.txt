cmake_minimum_required(VERSION 3.10)
project(minishell_tests C)

# Enable testing with CTest
enable_testing()

# Option to enable or disable Valgrind
option(ENABLE_VALGRIND "Enable memory leak testing with Valgrind" ON)

# Set project directories relative to the test_minishell directory
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Include directories
set(INCLUDE_DIR "${ROOT_DIR}/include")
set(LIBGC_INCLUDE_DIR "${ROOT_DIR}/lib/libgc/include")
set(LIBFT_INCLUDE_DIR "${ROOT_DIR}/lib/libft/include")
include_directories(
    ${INCLUDE_DIR}
    ${LIBGC_INCLUDE_DIR}
    ${LIBFT_INCLUDE_DIR}
    ${CHECK_INCLUDE_DIRS}
)

# Find Check library using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)
include_directories(${CHECK_INCLUDE_DIRS})

# List of test source files
set(TEST_SOURCES
    ${TEST_SRC_DIR}/test_main.c
    ${TEST_SRC_DIR}/run_minishell_command.c
)

# Create the test executable
add_executable(minishell_tests ${TEST_SOURCES})

# Set library paths
set(LIBGC_LIB "${ROOT_DIR}/lib/libgc/libgc.a")
set(LIBFT_LIB "${ROOT_DIR}/lib/libft/libft.a")

# Link the Check library, libgc, and libft
target_link_libraries(
    minishell_tests
    ${CHECK_LIBRARIES}
    ${LIBGC_LIB}
    ${LIBFT_LIB}
)

# Add test
if(ENABLE_VALGRIND)
    # Use Valgrind to run the tests
    find_program(VALGRIND_EXECUTABLE valgrind)
    if(VALGRIND_EXECUTABLE)
        add_test(
            NAME run_tests
            COMMAND ${VALGRIND_EXECUTABLE} --leak-check=full $<TARGET_FILE:minishell_tests>
        )
    else()
        message(WARNING "Valgrind executable not found; running tests without Valgrind")
        add_test(
            NAME run_tests
            COMMAND $<TARGET_FILE:minishell_tests>
        )
    endif()
else()
    # Run the test normally
    add_test(
        NAME run_tests
        COMMAND $<TARGET_FILE:minishell_tests>
    )
endif()


#cmake_minimum_required(VERSION 3.10)
#project(minishell_tests C)
#
## Enable testing with CTest
#enable_testing()
#
## Option to enable or disable Valgrind
#option(ENABLE_VALGRIND "Enable memory leak testing with Valgrind" ON)
#
## Set project directories relative to the test_minishell directory
#set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
#set(TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
#
## Include directories
#set(INCLUDE_DIR "${ROOT_DIR}/include")
#set(LIBGC_INCLUDE_DIR "${ROOT_DIR}/lib/libgc/include")
#set(LIBFT_INCLUDE_DIR "${ROOT_DIR}/lib/libft/include")
#include_directories(
#    ${INCLUDE_DIR}
#    ${LIBGC_INCLUDE_DIR}
#    ${LIBFT_INCLUDE_DIR}
#)
#
## Find Check library using pkg-config
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(CHECK REQUIRED check)
#include_directories(${CHECK_INCLUDE_DIRS})
#
## Add custom target to build minishell
#add_custom_target(build_minishell ALL
#    COMMAND make test
#    WORKING_DIRECTORY ${ROOT_DIR}
#)
#
## List of test source files
#set(TEST_SOURCES
#    ${TEST_SRC_DIR}/test_main.c
#    ${TEST_SRC_DIR}/run_minishell_command.c
#)
#
## Create the test executable
#add_executable(minishell_tests ${TEST_SOURCES})
#
## Set library paths
#set(LIBGC_LIB "${ROOT_DIR}/lib/libgc/libgc.a")
#set(LIBFT_LIB "${ROOT_DIR}/lib/libft/libft.a")
#
## Link the Check library, libgc, and libft
#target_link_libraries(
#    minishell_tests
#    ${CHECK_LIBRARIES}
#    ${LIBGC_LIB}
#    ${LIBFT_LIB}
#)
#
## Ensure the test executable depends on build_minishell
#add_dependencies(minishell_tests build_minishell)
#
## Add test
#if(ENABLE_VALGRIND)
#    # Use Valgrind to run the tests
#    find_program(VALGRIND_EXECUTABLE valgrind)
#    if(VALGRIND_EXECUTABLE)
#        add_test(
#            NAME run_tests
#            COMMAND ${VALGRIND_EXECUTABLE} --leak-check=full $<TARGET_FILE:minishell_tests>
#        )
#    else()
#        message(WARNING "Valgrind executable not found; running tests without Valgrind")
#        add_test(
#            NAME run_tests
#            COMMAND $<TARGET_FILE:minishell_tests>
#        )
#    endif()
#else()
#    # Run the test normally
#    add_test(
#        NAME run_tests
#        COMMAND $<TARGET_FILE:minishell_tests>
#    )
#endif()
#
