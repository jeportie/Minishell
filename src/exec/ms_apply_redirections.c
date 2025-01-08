/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_apply_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:00:21 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/07 12:29:46 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static int	st_dup_helper(int fd, int std_fileno)
{
	if (dup2(fd, std_fileno) == -1)
	{
		dprintf(STDERR_FILENO, "minishell: dup2 error on STDIN\n");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int	ms_apply_redirections(t_redir *redir_list)
{
	t_redir	*current;
	int		fd;

	current = redir_list;
	while (current)
	{
		fd = ms_open_redir_file(current->type, current->filename);
		if (fd == -1)
			return (-1);
		if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (!st_dup_helper(fd, STDIN_FILENO))
				return (-1);
		}
		else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			if (!st_dup_helper(fd, STDOUT_FILENO))
				return (-1);
		current = current->next;
	}
	return (0);
}

/*
			//if (current->type == REDIR_HEREDOC)
			//	unlink(current->filename);
*/
