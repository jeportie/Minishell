/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_mode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:49:03 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/05 11:06:12 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/minishell.h"

static void	ms_heredoc_child(const char *delimiter, int write_fd)
{
	char	*line;
	size_t	len;
	int		tty_read_fd;
	int		tty_write_fd;

	// Reset signal handlers
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);

	// Open /dev/tty for reading
	tty_read_fd = open("/dev/tty", O_RDONLY);
	if (tty_read_fd == -1)
	{
		perror("minishell: Unable to open /dev/tty");
		close(write_fd);
		exit(EXIT_FAILURE);
	}
	// Open /dev/tty for writing
	tty_write_fd = open("/dev/tty", O_WRONLY);
	if (tty_write_fd == -1)
	{
		perror("minishell: Unable to open /dev/tty");
		close(tty_read_fd);
		close(write_fd);
		exit(EXIT_FAILURE);
	}

	// Duplicate tty_fd to STDIN_FILENO
	if (dup2(tty_read_fd, STDIN_FILENO) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: dup2 failed");
		close(tty_read_fd);
		close(tty_write_fd);
		close(write_fd);
		exit(EXIT_FAILURE);
	}
	close(tty_read_fd); // Close the now-duplicated tty_fd

	len = ft_strlen(delimiter);

	// Remove or modify this debug print to avoid confusion
	// printf("Heredoc child exiting with status 0\n");

	while (1)
	{
		if (write(tty_write_fd, "> ", 2) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: write to tty failed\n");
			close(tty_write_fd);
			close(write_fd);
			exit(EXIT_FAILURE);
		}
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, len) == 0 && line[len] == '\n')
		{
			free(line);
			break ;
		}
		if (write(write_fd, line, ft_strlen(line)) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: error: write to pipe failed\n");
			close(tty_write_fd);
			close(write_fd);
			exit(EXIT_FAILURE);
		}
		free(line);
	}
	close(tty_write_fd);
	close(write_fd);
	exit(0);
}

static int	ms_heredoc_parent(pid_t pid, int fd[2], t_exec_context *context,
	t_gc *gcl)
{
	int	status;
	int	exit_status;
	int	sig;

	close(fd[1]);
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
	context->exit_status = exit_status;
	if (exit_status != 0)
	{
		close(fd[0]);
		context->exit_status = exit_status;
		return (-1);
	}
	if (context->stdin_fd != STDIN_FILENO)
		close(context->stdin_fd);
	context->stdin_fd = fd[0];
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
		ms_heredoc_parent(pid, pipefd, context, gcl);
	return (0);
}
