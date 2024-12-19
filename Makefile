# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeportie <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/12 14:15:40 by jeportie          #+#    #+#              #
#    Updated: 2024/12/03 17:10:21 by jeportie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

### BEGIN AUTO GENERATED FILES ###
# List of source files:
SRC = \
  src/exec/ms_execute_command.c \
  src/exec/ms_concat_path.c \
  src/exec/ms_execute_external.c \
  src/exec/ms_handle_error.c \
  src/exec/ms_execute_subshell.c \
  src/exec/ms_handle_redirections.c \
  src/exec/ms_parse_cmd_path.c \
  src/exec/ms_heredoc_child.c \
  src/exec/ms_execute_ast.c \
  src/exec/ms_heredoc_mode.c \
  src/exec/ms_execute_logical.c \
  src/exec/ms_pipeline_helper.c \
  src/exec/ms_execute_pipeline.c \
  src/signal/ms_init_child_cmd_signal.c \
  src/signal/ms_init_parent_cmd_signal.c \
  src/signal/ms_init_std_signal.c \
  src/signal/rl_event_dummy.c \
  src/signal/ms_heredoc_signal.c \
  src/signal/ms_stop_std_signal.c \
  src/syntax_error/check_error.c \
  src/syntax_error/check_reinit.c \
  src/syntax_error/operator_gestion.c \
  src/syntax_error/quote_gestion.c \
  src/syntax_error/ms_syntax_error.c \
  src/syntax_error/redir_gestion.c \
  src/syntax_error/parenthesis_gestion.c \
  src/expand/ms_expand_args.c \
  src/expand/ms_expand_args_utils.c \
  src/expand/ms_manage_arg_expand.c \
  src/expand/ms_value_assign.c \
  src/expand/ms_expand_wild.c \
  src/expand/ms_wildcard_helper.c \
  src/expand/ms_expand_utils.c \
  src/process/clean_manager.c \
  src/process/safe_close.c \
  src/process/add_process.c \
  src/process/print_proc_info.c \
  src/process/redirect_fd.c \
  src/process/safe_pipe.c \
  src/process/init_proc_manager.c \
  src/process/clean_process.c \
  src/process/safe_open.c \
  src/process/back_to_parent_stdout.c \
  src/process/safe_fork.c \
  src/shell/ms_set_env_value.c \
  src/shell/ms_get_user_input.c \
  src/shell/ms_env_add_back.c \
  src/shell/ms_get_env_value.c \
  src/shell/ms_init_env.c \
  src/shell/ms_getenv.c \
  src/shell/ms_get_envp.c \
  src/shell/init_fix_value.c \
  src/shell/ms_init_shell.c \
  src/shell/ms_env_create_node.c \
  src/ast/create_node.c \
  src/ast/parse_subshell.c \
  src/ast/parse_pipeline.c \
  src/ast/utils2.c \
  src/ast/ms_parse_tokens.c \
  src/ast/parse_command.c \
  src/ast/parse_logical.c \
  src/ast/parse_redirection.c \
  src/ast/create_command_node.c \
  src/ast/print_ast.c \
  src/ast/utils.c \
  src/global.c \
  src/builtins/ms_export.c \
  src/builtins/ms_exit.c \
  src/builtins/ms_echo.c \
  src/builtins/ms_env.c \
  src/builtins/export_utils.c \
  src/builtins/ms_cd.c \
  src/builtins/echec_malloc.c \
  src/builtins/ms_unset.c \
  src/builtins/builtins_utils.c \
  src/builtins/copy_env_list.c \
  src/builtins/ms_pwd.c \
  src/token/lst_tokens.c \
  src/token/tokenize_frame.c \
  src/token/ms_tokenize.c \
  src/token/tokenize_operator.c \
  src/token/tokenize_word.c \
  src/token/utils.c \
  src/token/tokenize_word_helper.c 
### END AUTO GENERATED FILES ###

# **************************************************************************** #
#                               Compiler & Flags                               #
# **************************************************************************** #

CC = 		cc
CFLAGS = 	-Wall -Wextra -Werror
VFLAGS = 	-g3 -fPIC 
SANITIZE = 	-g3 -fPIC -fsanitize=thread
VALG =		valgrind --leak-check=full --show-leak-kinds=all \
			--track-origins=yes --error-exitcode=1 \
			--suppressions=assets/supp.supp
HELG =      valgrind --tool=helgrind --history-level=full \
			--track-lockorders=yes --show-below-main=yes --free-is-write=yes

LDFLAGS = -lreadline -L./lib/libgc -lgc -L./lib/libft -lft
DEPFLAGS =  -MMD -MP
INCLUDES = -I./include -I./lib/libgc/include -I./lib/libft/include

SRC_DIR = 	src
OBJ_DIR = 	obj

# **************************************************************************** #
#                                 Files                                        #
# **************************************************************************** #

# Object and Dependency Files
OBJ = 		$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS = 		$(OBJ:.o=.d)

# **************************************************************************** #
#                                 Colors                                       #
# **************************************************************************** #

RESET   = \033[0m
GREEN   = \033[32m
BLUE    = \033[34m
YELLOW  = \033[33m
RED     = \033[31m
BOLD    = \033[1m

# **************************************************************************** #
#                           Other Variables                                    #
# **************************************************************************** #

TOTAL_STEPS := $(words $(OBJ))
SCRIPT_URL = https://github.com/jeromeDev94/make_interface.git

# **************************************************************************** #
#                                Main Rules                                    #
# **************************************************************************** #

VERBOSE ?= @

# Default target
run-prompt: download-script
	@./make_interface/exec/make_prompt

all: $(NAME)

$(NAME): $(OBJ) $(OBJ_DIR)/main.o
	make re -C lib/libgc
	make re -C lib/libft
	@echo "Compiling $(NAME)..."
	$(CC) $(CFLAGS) $(OBJ) $(OBJ_DIR)/main.o -o $(NAME) $(LDFLAGS) > .compile.log 2>&1
	@if [ "$(VERBOSE)" = "@" ]; then \
		./make_interface/exec/progress $(words $(OBJ)) .compile.log; \
	else \
		cat .compile.log; \
	fi
	@echo "Compilation complete."

classic: VERBOSE =
classic: $(NAME)

# Ensure obj subdirectories for object files exist
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)  # Create subdirectories as needed
	$(VERBOSE)$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(VERBOSE)$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS)


# **************************************************************************** #
#                                Clean-up                                      #
# **************************************************************************** #

clean:
	@echo "Cleaning object files"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleaning $(NAME)"
	@rm -f $(NAME)

tclean:
	@bash ./make_interface/exec/clean_test.sh

fullclean: fclean
	@echo "Cleaning Interface"
	@rm -rf make_interface .download.log .compile.log tags

# **************************************************************************** #
#                                Other Rules                                   #
# **************************************************************************** #

re: fclean all

run:
	@./$(NAME) $(filter-out $@,$(MAKECMDGOALS))

helgrind:
	@$(HELG) ./$(NAME) $(filter-out $@,$(MAKECMDGOALS))

valgrind:
	@$(VALG) ./$(NAME) $(filter-out $@,$(MAKECMDGOALS))

# Thread sanitizer rule
san: CFLAGS += $(SANITIZE)
san: CFLAGS -= $(VFLAGS)
san: CC = clang
san: re

val: CFLAGS -= $(SANITIZE)
val: CFLAGS += $(VFLAGS) 
val: CC = cc
val: re

default: CFLAGS -= $(SANITIZE)
default: CFLAGS -= $(VFLAGS)
default: CC = cc
default: re

# **************************************************************************** #
#                               Utility Rules                                  #
# **************************************************************************** #

norm:
	@norminette include/ main.c src/

todo:
	@vim .$(NAME).todo.md

uml:
	@./make_interface/exec/interactive_select 

build:
	@bash ./make_interface/config/build.sh
	make update



update:
	./make_interface/exec/update_makefile.sh

git:
	@tig

debug:
	@echo "Searching for '# define DEBUG' in headers..."
	@grep -Hn '# define DEBUG' include/*.h || echo "No DEBUG flag found."
	@if grep -q '# define DEBUG' include/*.h; then \
		read -p "Do you want to turn DEBUG on (1) or off (0)? " debug_value; \
		if [ $$debug_value -eq 1 ]; then \
			echo "Setting DEBUG to $$debug_value..."; \
			sed -i 's/# define DEBUG.*/# define DEBUG 1 /' include/*.h; \
			echo "DEBUG flag updated to $$debug_value."; \
		elif [ $$debug_value -eq 0 ]; then \
			echo "Setting DEBUG to $$debug_value..."; \
			sed -i 's/# define DEBUG.*/# define DEBUG 0 /' include/*.h; \
			echo "DEBUG flag updated to $$debug_value."; \
		else \
			echo "Invalid input. Please enter 0 or 1."; \
		fi \
	else \
		echo "No '# define DEBUG' found in headers."; \
	fi

nodebug:
	@grep -Hn '# define DEBUG' include/*.h || echo "No DEBUG flag found."
	@if grep -q '# define DEBUG' include/*.h; then \
		sed -i 's/# define DEBUG.*/# define DEBUG 0 /' include/*.h; \
	fi

help:
	@echo "$(BOLD)$(YELLOW)------------------- $(NAME) project Makefile (42 school) ---------------------------------$(RESET)"
	@echo ""
	@echo "Welcome to the 42 School Make Interface - 42MI"
	@echo ""
	@echo "$(BOLD)$(BLUE)Usage:$(RESET)"
	@echo "  Make> [target]"
	@echo ""
	@echo "$(BOLD)$(BLUE)Available targets (++ = takes args after target):$(RESET)"
	@echo "$(GREEN)  norm           $(RESET)- Runs norminette on the source files."
	@echo "$(GREEN)  all            $(RESET)- Compiles the $(NAME) project."
	@echo "$(GREEN)  classic        $(RESET)- Compiles the $(NAME) project with all compilation logs."
	@echo "$(GREEN)  val            $(RESET)- Compiles the $(NAME) project with valgrind compilation flags."
	@echo "$(GREEN)  san            $(RESET)- Clang compiles with thread sanitizer enabled. $(RED)!DO NOT USE VALGRIND.$(RESET)"
	@echo "$(GREEN)  default        $(RESET)- Unable sanitizer thread and compiles with cc (Valgrind OK)"
	@echo "$(GREEN)  run ++         $(RESET)- Executes the compiled $(NAME) program with optional arguments."
	@echo "$(GREEN)  helgrind ++    $(RESET)- Runs Valgrind's Helgrind tool for thread debugging."
	@echo "$(GREEN)  valgrind ++    $(RESET)- Runs Valgrind's leak-check for memory leaks debugging."
	@echo "$(GREEN)  debug          $(RESET)- Set the DEBUG flag in the project .h file(s)."
	@echo "$(GREEN)  clean          $(RESET)- Removes object files."
	@echo "$(GREEN)  fclean         $(RESET)- Removes object files and the executable."
	@echo "$(GREEN)  tclean         $(RESET)- Removes test files and the test executables."
	@echo "$(GREEN)  fullclean      $(RESET)- Removes the interface, object files, and executable."
	@echo "$(GREEN)  re             $(RESET)- Cleans and recompiles the project."
	@echo "$(GREEN)  build          $(RESET)- Run the Makefile builder script."
	@echo "$(GREEN)  update         $(RESET)- Update the .c source files list in Makefile."
	@echo "$(GREEN)  git            $(RESET)- Opens git interface in vim."
	@echo "$(GREEN)  uml            $(RESET)- Enter in the UML schematics menu of the projects."
	@echo "$(GREEN)  calendar       $(RESET)- Displays the project calendar."
	@echo "$(GREEN)  todo           $(RESET)- Displays the project todo list."
	@echo "$(GREEN)  run-prompt     $(RESET)- Starts a custom Make prompt interface."
	@echo "$(GREEN)  help           $(RESET)- Displays this help message."
	@echo ""
	@echo "$(BOLD)$(YELLOW)-----------------------------------------------------------------------------------------$(RESET)"

download-script:
	@if [ ! -d make_interface ]; then \
		echo "Downloading make_interface script..."; \
		git clone $(SCRIPT_URL) > .download.log 2>&1;\
		./make_interface/exec/progress 10 .download.log; \
		chmod +x make_interface/make_prompt; \
		echo "Script downloaded !"; \
		make -s help; \
	fi

# Catch-all rule to prevent errors with undefined targets
%:
	@:

### PHONY TARGETS ###
.PHONY: all clean fclean re val san download-script run-prompt run norm valgrind helgrind todo calendar debug
