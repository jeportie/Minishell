/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_ast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 09:32:42 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/06 15:02:46 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	init_context(t_exec_context *data, t_shell *shell)
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

bool	is_builtin_that_must_run_in_parent(t_cmd_node *cmd_node)
{
	if (!cmd_node || !cmd_node->argv[0])
		return (false);
	if (ft_strncmp(cmd_node->argv[0], "cd", 3) == 0)
		return (true);
	if (ft_strncmp(cmd_node->argv[0], "export", 7) == 0)
		return (true);
	if (ft_strncmp(cmd_node->argv[0], "unset", 6) == 0)
		return (true);
	if (ft_strncmp(cmd_node->argv[0], "exit", 5) == 0)
		return (true);
	return (false);
}

static int	redir_in_command(t_ast_node *node, t_exec_context *context,
		t_redir *redir_list)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	context->redir_list = redir_list;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (ms_apply_redirections(context->redir_list) != 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		context->redir_list = NULL;
		context->shell->error_code = 1;
		return (1);
	}
	ret = ms_execute_command(&node->data.command, context, context->shell->gcl);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	context->redir_list = NULL;
	return (ret);
}

int	ms_execute_redirection(t_ast_node *node, t_exec_context *context)
{
	t_redir	*redir_list;

	redir_list = ms_collect_redirections(node, context->shell->gcl,
			context->shell);
	while (node && (node->type == NODE_REDIRECT_IN
			|| node->type == NODE_REDIRECT_OUT
			|| node->type == NODE_REDIRECT_APPEND
			|| node->type == NODE_REDIRECT_HEREDOC))
	{
		if (node->type == NODE_REDIRECT_HEREDOC)
			node = node->data.heredoc.child;
		else
			node = node->data.redirect.child;
	}
	if (!node)
		return (ms_handle_error("Redirection with no command\n", 1,
				context->shell->gcl));
	if (node->type == NODE_COMMAND)
		return (redir_in_command(node, context, redir_list));
	else
	{
		context->redir_list = redir_list;
		return (ms_execute_ast(node, context));
	}
}

int	ms_execute_ast(t_ast_node *node, t_exec_context *context)
{
	if (!node)
		return (ms_handle_error("Error: Null AST node.\n", -1,
				context->shell->gcl));
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (ms_execute_logical(node, context));
	else if (node->type == NODE_SUBSHELL)
		return (ms_execute_subshell(&node->data.subshell, context));
	else if (node->type == NODE_PIPE)
		return (ms_execute_pipeline(node, context));
	else if (node->type == NODE_COMMAND)
		return (ms_execute_command(&node->data.command, context,
				context->shell->gcl));
	else
		return (ms_execute_redirection(node, context));
}
