/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:52:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/27 21:16:17 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"
#include "include/syntax.h"
#include "include/tokenize.h"
#include "include/ast.h"
#include "include/exec.h"
#include "lib/libft/include/libft.h"
#include <fcntl.h>

void	print_token_delimit(t_token *tokens)
{
	int	fd;

	if (DEBUG == 0)
		return ;
	fd = open(PRINT_INFOS, O_WRONLY | O_TRUNC | O_CREAT, COPY_MODE);
	ft_dprintf(fd, "-----------------------------------------------------");
	ft_dprintf(fd, "---------------------------\nTOKENS:\n");
	print_token(tokens);
	close(fd);
}

void	print_ast_delimit(t_ast_node *root)
{
	int	fd;

	if (DEBUG == 0)
		return ;
	fd = open(PRINT_INFOS, O_WRONLY | O_APPEND, COPY_MODE);
	ft_dprintf(fd, "------------------------------------------------");
	ft_dprintf(fd, "--------------------------------\nAST:\n");
	print_ast(root, 0, "", 0);
	ft_dprintf(fd, "-------------------------------------------"
		"---------------------");
	ft_dprintf(fd, "----------------\n");
	ft_dprintf(fd, "Process Manager - Active Processes:\n");
	ft_dprintf(fd, "Title\t\tPID\tParent PID\tLevel"
		"\tFD_in\tFD_out\tFD_err\tHeredoc\n");
	ft_dprintf(fd, "-------------------------"
		"---------------------------------------");
	ft_dprintf(fd, "----------------\n");
	close(fd);
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
	data->redirected = false;
	data->original_stdin = STDIN_FILENO;
	data->original_stdout = STDOUT_FILENO;
	data->tmpfile_counter = 0;
}

void	ms_preprocess_heredocs(t_exec_context *context)
{
	int	i;

	i = 0;
	while (i < 16 && context->shell->heredocs[i])
	{
		ms_heredoc_mode(context->shell->heredocs[i], context);
		i++;
	}
}

static void	run(t_shell *shell, t_token *tokens, t_ast_node *root)
{
	t_exec_context	context;

	tokens = ms_tokenize(shell->user_input, shell->gcl);
//	print_token_delimit(tokens);
	root = ms_parse_tokens(tokens, shell, shell->gcl);
	if (!root)
		return ;
//	print_ast_delimit(root);
//	gc_collect(shell->gcl);
	init_context(&context, shell);
	ms_preprocess_heredocs(&context);
	ms_execute_ast(root, &context);
//	gc_collect(shell->gcl);
}

int	main(int argc, char **argv, char **envp)
{
	t_token *(tokens) = NULL;
	t_ast_node *(root) = NULL;
	t_shell (shell) = ms_init_shell(argc, argv, envp);
	shell.interactive_mode = isatty(STDIN_FILENO);
	while (1)
	{
		ms_init_std_signal();
		ms_get_user_input(&shell);
		if (!shell.user_input)
		{
			rl_clear_history();
			gc_cleanup(shell.gcl);
			if (shell.interactive_mode)
				ft_dprintf(1, "exit\n");
			exit (shell.error_code);
		}
		if (ms_syntax_error(shell.user_input))
			shell.error_code = 2;
		else
			run(&shell, tokens, root);
		ft_bzero(shell.heredocs, 16);
	}
	gc_cleanup(shell.gcl);
	return (shell.error_code = 333);
}
