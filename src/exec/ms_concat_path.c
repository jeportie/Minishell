/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_concat_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:57:10 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 10:55:19 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static void	st_check_alloc(void *ptr, t_gc *gcl)
{
	if (!ptr)
	{
		ft_dprintf(STDERR_FILENO, SHELL ": memory allocation error");
		gc_cleanup(gcl);
		exit(EXIT_FAILURE);
	}
}

char	*ms_concat_path(const char *path, const char *command, t_gc *gcl)
{
	char	*cmd_path;
	char	*tmp;

	tmp = gc_strjoin((char *)path, "/", gcl);
	st_check_alloc(tmp, gcl);
	cmd_path = gc_strjoin(tmp, (char *)command, gcl);
	st_check_alloc(cmd_path, gcl);
	return (cmd_path);
}

static int	st_redirect_fd(int old_fd, int new_fd)
{
	if (dup2(new_fd, old_fd) == -1)
		return (-1);
	return (0);
}

int	ms_init_io(int new_in, int new_out, int new_err)
{
	if (new_in != STDIN_FILENO)
	{
		if (st_redirect_fd(STDIN_FILENO, new_in) == -1)
			return (0);
	}
	if (new_out != STDOUT_FILENO)
	{
		if (st_redirect_fd(STDOUT_FILENO, new_out) == -1)
			return (0);
	}
	if (new_err != STDERR_FILENO)
	{
		if (st_redirect_fd(STDERR_FILENO, new_err) == -1)
			return (0);
	}
	return (1);
}
