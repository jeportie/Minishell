# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/projects/Minishell/test_src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/projects/Minishell/test_src/build

# Include any dependencies generated for this target.
include CMakeFiles/test_ms_stop_std_signal.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_ms_stop_std_signal.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_ms_stop_std_signal.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_ms_stop_std_signal.dir/flags.make

CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.o: CMakeFiles/test_ms_stop_std_signal.dir/flags.make
CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.o: /root/projects/Minishell/test_src/signal/test_ms_stop_std_signal.c
CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.o: CMakeFiles/test_ms_stop_std_signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/projects/Minishell/test_src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.o -MF CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.o.d -o CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.o -c /root/projects/Minishell/test_src/signal/test_ms_stop_std_signal.c

CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/projects/Minishell/test_src/signal/test_ms_stop_std_signal.c > CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.i

CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/projects/Minishell/test_src/signal/test_ms_stop_std_signal.c -o CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.s

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.o: CMakeFiles/test_ms_stop_std_signal.dir/flags.make
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.o: /root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.o: CMakeFiles/test_ms_stop_std_signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/projects/Minishell/test_src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.o -MF CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.o.d -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.o -c /root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c > CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.i

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.s

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.o: CMakeFiles/test_ms_stop_std_signal.dir/flags.make
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.o: /root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.o: CMakeFiles/test_ms_stop_std_signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/projects/Minishell/test_src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.o -MF CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.o.d -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.o -c /root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c > CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.i

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.s

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.o: CMakeFiles/test_ms_stop_std_signal.dir/flags.make
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.o: /root/projects/Minishell/src/init_shell/ms_get_user_input.c
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.o: CMakeFiles/test_ms_stop_std_signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/projects/Minishell/test_src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.o -MF CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.o.d -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.o -c /root/projects/Minishell/src/init_shell/ms_get_user_input.c

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/projects/Minishell/src/init_shell/ms_get_user_input.c > CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.i

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/projects/Minishell/src/init_shell/ms_get_user_input.c -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.s

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.o: CMakeFiles/test_ms_stop_std_signal.dir/flags.make
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.o: /root/projects/Minishell/src/init_shell/ms_init_env.c
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.o: CMakeFiles/test_ms_stop_std_signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/projects/Minishell/test_src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.o -MF CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.o.d -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.o -c /root/projects/Minishell/src/init_shell/ms_init_env.c

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/projects/Minishell/src/init_shell/ms_init_env.c > CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.i

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/projects/Minishell/src/init_shell/ms_init_env.c -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.s

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.o: CMakeFiles/test_ms_stop_std_signal.dir/flags.make
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.o: /root/projects/Minishell/src/init_shell/ms_init_shell.c
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.o: CMakeFiles/test_ms_stop_std_signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/projects/Minishell/test_src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.o -MF CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.o.d -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.o -c /root/projects/Minishell/src/init_shell/ms_init_shell.c

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/projects/Minishell/src/init_shell/ms_init_shell.c > CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.i

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/projects/Minishell/src/init_shell/ms_init_shell.c -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.s

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.o: CMakeFiles/test_ms_stop_std_signal.dir/flags.make
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.o: /root/projects/Minishell/src/signal/ms_init_std_signal.c
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.o: CMakeFiles/test_ms_stop_std_signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/projects/Minishell/test_src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.o -MF CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.o.d -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.o -c /root/projects/Minishell/src/signal/ms_init_std_signal.c

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/projects/Minishell/src/signal/ms_init_std_signal.c > CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.i

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/projects/Minishell/src/signal/ms_init_std_signal.c -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.s

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.o: CMakeFiles/test_ms_stop_std_signal.dir/flags.make
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.o: /root/projects/Minishell/src/signal/ms_stop_std_signal.c
CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.o: CMakeFiles/test_ms_stop_std_signal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/projects/Minishell/test_src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.o -MF CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.o.d -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.o -c /root/projects/Minishell/src/signal/ms_stop_std_signal.c

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/projects/Minishell/src/signal/ms_stop_std_signal.c > CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.i

CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/projects/Minishell/src/signal/ms_stop_std_signal.c -o CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.s

# Object files for target test_ms_stop_std_signal
test_ms_stop_std_signal_OBJECTS = \
"CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.o" \
"CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.o" \
"CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.o" \
"CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.o" \
"CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.o" \
"CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.o" \
"CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.o" \
"CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.o"

# External object files for target test_ms_stop_std_signal
test_ms_stop_std_signal_EXTERNAL_OBJECTS =

test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/signal/test_ms_stop_std_signal.c.o
test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_add_back.c.o
test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/env_lst_utils/ms_env_create_node.c.o
test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_get_user_input.c.o
test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_env.c.o
test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/init_shell/ms_init_shell.c.o
test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_init_std_signal.c.o
test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/root/projects/Minishell/src/signal/ms_stop_std_signal.c.o
test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/build.make
test_ms_stop_std_signal: /root/projects/Minishell/test_src/../lib/libgc/libgc.a
test_ms_stop_std_signal: /root/projects/Minishell/test_src/../lib/libft/libft.a
test_ms_stop_std_signal: CMakeFiles/test_ms_stop_std_signal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/root/projects/Minishell/test_src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking C executable test_ms_stop_std_signal"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_ms_stop_std_signal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_ms_stop_std_signal.dir/build: test_ms_stop_std_signal
.PHONY : CMakeFiles/test_ms_stop_std_signal.dir/build

CMakeFiles/test_ms_stop_std_signal.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_ms_stop_std_signal.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_ms_stop_std_signal.dir/clean

CMakeFiles/test_ms_stop_std_signal.dir/depend:
	cd /root/projects/Minishell/test_src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/projects/Minishell/test_src /root/projects/Minishell/test_src /root/projects/Minishell/test_src/build /root/projects/Minishell/test_src/build /root/projects/Minishell/test_src/build/CMakeFiles/test_ms_stop_std_signal.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test_ms_stop_std_signal.dir/depend
