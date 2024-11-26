/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:52:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/25 15:06:59 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"
#include "include/syntax.h"
#include "include/tokenize.h"
#include "include/ast.h"
#include "include/exec.h"
#include "include/process.h"

static void print_token_delimit(t_token *tokens)
{
	printf("-----------------------------------------------------");
	printf("---------------------------\nTOKENS:\n");
	print_token(tokens);
}

static void print_ast_delimit(t_ast_node *root)
{
	printf("------------------------------------------------");
	printf("--------------------------------\nAST:\n");
	print_ast(root, 0, "", 0);
	printf("----------------------------------------------------------------");
	printf("----------------\n");
}

static void	init_context(t_exec_context *data, t_shell *shell)
{
	data->stdin_fd = STDIN_FILENO;
	data->stdout_fd = STDOUT_FILENO;
	data->stderr_fd = STDERR_FILENO;
	data->shell = shell;
	data->is_subprocess = false;
	data->exit_status = 0;
	data->child_lvl = 0;
}

static void	run(t_shell *shell, t_token *tokens, t_ast_node *root)
{
	t_exec_context	context;
	t_proc_manager	*proc_manager;

	tokens = ms_tokenize(shell->user_input, shell->gcl);
	print_token_delimit(tokens);
	root = ms_parse_tokens(tokens, shell->gcl);
	if (!root)
	{
		gc_cleanup(shell->gcl);
		exit(EXIT_FAILURE);
	}
	print_ast_delimit(root);
	init_context(&context, shell);
	proc_manager = init_manager(shell->gcl);
	ms_execute_ast(root, &context, proc_manager);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_token	*tokens;

	t_ast_node *(root) = NULL;
	shell = ms_init_shell(argc, argv, envp);
	tokens = NULL;
	while (1)
	{
		ms_init_std_signal();
		ms_get_user_input(&shell);
		if (!shell.user_input)
		{
			rl_clear_history();
			gc_cleanup(shell.gcl);
			free(shell.gcl);
			exit (shell.error_code);
		}
		if (ms_syntax_error(shell.user_input))
			shell.error_code = 2;
		else
			run(&shell, tokens, root);
	}
	shell.error_code = 666;
	return (shell.error_code);
}
