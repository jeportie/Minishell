/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:12:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/03 16:41:43 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static int	ms_open_file(const char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
		ft_dprintf(STDERR_FILENO, "Minishell: Error: open failed.\n");
	return (fd);
}

int		ms_handle_redirections(t_ast_node *node, t_exec_context *context)
{
	int				fd;
	t_redirect_node	*redir_node;

	redir_node = &node->data.redirect;
	if (node->type == NODE_REDIRECT_IN)
	{
		fd = ms_open_file(redir_node->filename, O_RDONLY, 0);
		if (fd == -1)
			return (-1);
		if (context->stdin_fd != STDIN_FILENO)
			close(context->stdin_fd);
		context->stdin_fd = fd;
	}
	else if (node->type == NODE_REDIRECT_OUT)
	{
		fd = ms_open_file(redir_node->filename, O_WRONLY | O_CREAT | O_TRUNC, COPY_MODE);
		if (fd == -1)
			return (-1);
		if (context->stdout_fd != STDOUT_FILENO)
			close(context->stdout_fd);
		context->stdout_fd = fd;
	}
	else if (node->type == NODE_REDIRECT_APPEND)
	{
		fd = ms_open_file(redir_node->filename, O_WRONLY | O_CREAT | O_APPEND, COPY_MODE);
		if (fd == -1)
			return (-1);
		if (context->stdout_fd != STDOUT_FILENO)
			close(context->stdout_fd);
		context->stdout_fd = fd;
	}
	else if (node->type == NODE_REDIRECT_HEREDOC)
	{
		if (ms_heredoc_mode(redir_node->filename, context, context->shell->gcl))
		{
			ft_dprintf(STDERR_FILENO, "Minishell: Error: heredoc failed.\n");
			context->stdin_fd = g_signal;
			return (-1);
		}
	}
	else
	{
		ft_dprintf(STDERR_FILENO, "Minishell: Error: unsupported redirection type.\n");
		return (-1);
	}
	if (redir_node->child)
		return (ms_handle_redirections(redir_node->child, context));
	return (0);
}
