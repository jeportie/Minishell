/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:07:45 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/05 18:28:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	safe_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: error: pipe failed\n");
		return (-1);
	}
	return (0);
}

int	safe_open(const char *filepath, int flags, int mode)
{
	int		fd;

	fd = open(filepath, flags, mode);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "Open failed\n");
		return (-1);
	}
	return (fd);
}

int	safe_close(int fd)
{
	if (close(fd) == -1)
	{
		ft_dprintf(STDERR_FILENO, "Close failed\n");
		return (-1);
	}
	return (fd);
}

bool	is_redirect_node(t_ast_node *node)
{
	return (node->type == NODE_REDIRECT_IN
		|| node->type == NODE_REDIRECT_OUT
		|| node->type == NODE_REDIRECT_APPEND
		|| node->type == NODE_REDIRECT_HEREDOC);
}
