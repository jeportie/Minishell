/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_mode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:49:03 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/03 16:32:03 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static void	ms_heredoc_child(const char *delimiter, int write_fd)
{
	char	*line;
	size_t	len;
	t_gc	*child_gcl;

	child_gcl = gc_init();
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	len = ft_strlen(delimiter);
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, len) == 0 && line[len] == '\n')
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		free(line);
	}
	close(write_fd);
	gc_cleanup(child_gcl);
	exit(0);
}

static int	ms_heredoc_parent(pid_t pid, int read_fd, t_exec_context *context,
	t_gc *gcl)
{
	int	status;
	int	exit_status;
	int	sig;

	close(read_fd + 1);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		exit_status = 128 + sig;
		if (sig == SIGINT)
			g_signal = 130;
	}
	if (exit_status != 0)
	{
		close(read_fd);
		context->exit_status = exit_status;
		return (-1);
	}
	if (context->stdin_fd != STDIN_FILENO)
		close(context->stdin_fd);
	context->stdin_fd = read_fd;
	gc_fd_register(context->stdin_fd, gcl);
	return (0);
}

int	ms_heredoc_mode(const char *delimiter, t_exec_context *context, t_gc *gcl)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe failed");
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork failed");
		return (-1);
	}
	else if (pid == 0)
		ms_heredoc_child(delimiter, pipefd[1]);
	else
		return (ms_heredoc_parent(pid, pipefd[0], context, gcl));
	return (0);
}
