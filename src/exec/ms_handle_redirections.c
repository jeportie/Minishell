/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_redirections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:12:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/05 18:31:01 by jeportie         ###   ########.fr       */
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
	t_heredoc_node	*heredoc_node;

	redir_node = &node->data.redirect;
	heredoc_node = &node->data.heredoc;
	if (node->type == 2)
	{
		fd = ms_open_file(redir_node->filename, O_RDONLY, 0);
		if (fd == -1)
			return (-1);
		if (context->stdin_fd != STDIN_FILENO)
			close(context->stdin_fd);
		context->stdin_fd = fd;
	}
	else if (node->type == 3)
	{
		fd = ms_open_file(redir_node->filename, O_WRONLY | O_CREAT | O_TRUNC, COPY_MODE);
		if (fd == -1)
			return (-1);
		if (context->stdout_fd != STDOUT_FILENO)
			close(context->stdout_fd);
		context->stdout_fd = fd;
	}
	else if (node->type == 4)
	{
		fd = ms_open_file(redir_node->filename, O_WRONLY | O_CREAT | O_APPEND, COPY_MODE);
		if (fd == -1)
			return (-1);
		if (context->stdout_fd != STDOUT_FILENO)
			close(context->stdout_fd);
		context->stdout_fd = fd;
	}
	else if (node->type == 5)
	{
		if (ms_heredoc_mode(heredoc_node->delimiter, context, context->shell->gcl) != 0)
		{
			ft_dprintf(STDERR_FILENO, "Minishell: Error: heredoc failed.\n");
		//	context->stdin_fd = g_signal;
			return (-1);
		}
	}
	else
	{
		ft_dprintf(STDERR_FILENO, "Minishell: Error: unsupported redirection type.\n");
		return (-1);
	}
	if ((redir_node->child->type >= 2 && redir_node->child->type <= 5))
		return (ms_handle_redirections(redir_node->child, context));
	return (0);
}
