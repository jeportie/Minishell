/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_concat_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:57:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/27 21:12:40 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static void	ms_check_alloc(void *ptr, t_gc *gcl)
{
	if (!ptr)
	{
		ft_dprintf(STDERR_FILENO, "error: minishell: memory allocation error");
		gc_cleanup(gcl);
		exit(EXIT_FAILURE);
	}
}

char	*ms_concat_path(const char *path, const char *command, t_gc *gcl)
{
	char	*cmd_path;
	char	*tmp;

	tmp = gc_strjoin((char *)path, "/", gcl);
	ms_check_alloc(tmp, gcl);
	cmd_path = gc_strjoin(tmp, (char *)command, gcl);
	ms_check_alloc(cmd_path, gcl);
	return (cmd_path);
}

int	redirect_fd(int old_fd, int new_fd)
{
	if (dup2(new_fd, old_fd) == -1)
		return (-1);
	return (0);
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
