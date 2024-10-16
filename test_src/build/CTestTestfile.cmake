# CMake generated Testfile for 
# Source directory: /root/projects/Minishell/test_src
# Build directory: /root/projects/Minishell/test_src/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ms_env_add_back "bash" "/root/projects/Minishell/test_src/run_test_with_valgrind.sh" "/usr/bin/valgrind" "/root/projects/Minishell/test_src/build/test_ms_env_add_back" "valgrind_ms_env_add_back.log" "/root/projects/Minishell/test_src/../assets/supp.supp")
set_tests_properties(ms_env_add_back PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "/root/projects/Minishell/test_src/CMakeLists.txt;93;add_test;/root/projects/Minishell/test_src/CMakeLists.txt;0;")
add_test(ms_env_create_node "bash" "/root/projects/Minishell/test_src/run_test_with_valgrind.sh" "/usr/bin/valgrind" "/root/projects/Minishell/test_src/build/test_ms_env_create_node" "valgrind_ms_env_create_node.log" "/root/projects/Minishell/test_src/../assets/supp.supp")
set_tests_properties(ms_env_create_node PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "/root/projects/Minishell/test_src/CMakeLists.txt;93;add_test;/root/projects/Minishell/test_src/CMakeLists.txt;0;")
add_test(ms_get_user_input "bash" "/root/projects/Minishell/test_src/run_test_with_valgrind.sh" "/usr/bin/valgrind" "/root/projects/Minishell/test_src/build/test_ms_get_user_input" "valgrind_ms_get_user_input.log" "/root/projects/Minishell/test_src/../assets/supp.supp")
set_tests_properties(ms_get_user_input PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "/root/projects/Minishell/test_src/CMakeLists.txt;93;add_test;/root/projects/Minishell/test_src/CMakeLists.txt;0;")
add_test(ms_init_env "bash" "/root/projects/Minishell/test_src/run_test_with_valgrind.sh" "/usr/bin/valgrind" "/root/projects/Minishell/test_src/build/test_ms_init_env" "valgrind_ms_init_env.log" "/root/projects/Minishell/test_src/../assets/supp.supp")
set_tests_properties(ms_init_env PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "/root/projects/Minishell/test_src/CMakeLists.txt;93;add_test;/root/projects/Minishell/test_src/CMakeLists.txt;0;")
add_test(ms_init_shell "bash" "/root/projects/Minishell/test_src/run_test_with_valgrind.sh" "/usr/bin/valgrind" "/root/projects/Minishell/test_src/build/test_ms_init_shell" "valgrind_ms_init_shell.log" "/root/projects/Minishell/test_src/../assets/supp.supp")
set_tests_properties(ms_init_shell PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "/root/projects/Minishell/test_src/CMakeLists.txt;93;add_test;/root/projects/Minishell/test_src/CMakeLists.txt;0;")
add_test(ms_init_std_signal "bash" "/root/projects/Minishell/test_src/run_test_with_valgrind.sh" "/usr/bin/valgrind" "/root/projects/Minishell/test_src/build/test_ms_init_std_signal" "valgrind_ms_init_std_signal.log" "/root/projects/Minishell/test_src/../assets/supp.supp")
set_tests_properties(ms_init_std_signal PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "/root/projects/Minishell/test_src/CMakeLists.txt;93;add_test;/root/projects/Minishell/test_src/CMakeLists.txt;0;")
add_test(ms_stop_std_signal "bash" "/root/projects/Minishell/test_src/run_test_with_valgrind.sh" "/usr/bin/valgrind" "/root/projects/Minishell/test_src/build/test_ms_stop_std_signal" "valgrind_ms_stop_std_signal.log" "/root/projects/Minishell/test_src/../assets/supp.supp")
set_tests_properties(ms_stop_std_signal PROPERTIES  TIMEOUT "5" _BACKTRACE_TRIPLES "/root/projects/Minishell/test_src/CMakeLists.txt;93;add_test;/root/projects/Minishell/test_src/CMakeLists.txt;0;")
