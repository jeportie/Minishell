/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_redirections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:12:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 14:00:40 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include <fcntl.h>

static int	ms_open_file(const char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
		ft_dprintf(STDERR_FILENO, "Minishell: Error: open failed.\n");
	return (fd);
}

static int	redirect_mode(t_ast_node *node, int flags, int *context_fd,
		int std_fd)
{
	int				fd;
	t_redirect_node	*redir_node;

	mode_t (mode) = 0;
	redir_node = &node->data.redirect;
	if (flags != O_RDONLY)
		mode = COPY_MODE;
	fd = ms_open_file(redir_node->filename, flags, mode);
	if (fd == -1)
		return (-1);
	if (*context_fd != std_fd)
		close(*context_fd);
	*context_fd = fd;
	return (0);
}

int	ms_handle_redirections(t_ast_node *node, t_exec_context *context, t_gc *gcl)
{
	t_redirect_node *(redir_node) = &node->data.redirect;
	t_heredoc_node *(heredoc_node) = &node->data.heredoc;
	if (node->type == 2)
		redirect_mode(node, O_RDONLY, &context->stdin_fd, STDIN_FILENO);
	else if (node->type == 3)
		redirect_mode(node, O_WRONLY | O_CREAT | O_TRUNC,
			&context->stdout_fd, STDOUT_FILENO);
	else if (node->type == 4)
		redirect_mode(node, O_WRONLY | O_CREAT | O_APPEND,
			&context->stdout_fd, STDOUT_FILENO);
	else if (node->type == 5)
	{
		if (ms_heredoc_mode(heredoc_node->delimiter, context, gcl) != 0)
			return (ms_handle_error("Minishell: Error: heredoc failed.\n",
					-1, gcl));
	}
	else
		return (ms_handle_error(
				"Minishell: Error: unsupported redirection type.\n", -1, gcl));
	if ((redir_node->child->type >= 2 && redir_node->child->type <= 5))
		return (ms_handle_redirections(redir_node->child, context, gcl));
	return (0);
}
