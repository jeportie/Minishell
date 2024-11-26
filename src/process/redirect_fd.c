/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:59:43 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/25 14:38:49 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"
#include "../../include/exec.h"

void	fork_init(t_fork_params *fork_params, t_exec_context *context,
			bool is_here_doc, char *title)
{
	fork_params->child_lvl = context->child_lvl + 1;
	fork_params->fd_in = context->stdin_fd;
	fork_params->fd_out = context->stdout_fd;
	fork_params->fd_error = context->stderr_fd;
	fork_params->is_heredoc = is_here_doc;
	fork_params->title = title;
}

int	init_io(int new_in, int new_out, int new_err)
{
	if (new_in != STDIN_FILENO)
	{
		if (redirect_fd(STDIN_FILENO, new_in) == -1)
			return (0);
	}
	if (new_out != STDOUT_FILENO)
	{
		if (redirect_fd(STDOUT_FILENO, new_out) == -1)
			return (0);
	}
	if (new_err != STDERR_FILENO)
	{
		if (redirect_fd(STDERR_FILENO, new_err) == -1)
			return (0);
	}
	return (1);
}

int	redirect_fd(int old_fd, int new_fd)
{
	if (dup2(new_fd, old_fd) == -1)
		return (-1);
	return (0);
}

int	restore_fd(int fd, int saved_fd)
{
	if (dup2(saved_fd, fd) == -1)
		return (-1);
	return (0);
}
