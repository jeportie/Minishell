/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:08:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/05 18:08:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static int	process_line(int write_fd, const char *delimiter, char **line_ptr)
{
	size_t	len;

	write(STDOUT_FILENO, "> ", 2);
	*line_ptr = get_next_line(STDIN_FILENO);
	if (!*line_ptr)
		return (1);
	len = strlen(*line_ptr);
	if (len > 0 && (*line_ptr)[len - 1] == '\n')
		(*line_ptr)[len - 1] = '\0';
	if (strcmp(*line_ptr, delimiter) == 0)
		return (0);
	if (write(write_fd, *line_ptr, strlen(*line_ptr)) == -1)
		return (-1);
	if (write(write_fd, "\n", 1) == -1)
		return (-1);
	return (2);
}

/* Refactored heredoc function */
static int	read_and_write_heredoc(int write_fd, const char *delimiter)
{
	char	*line;
	int		status;

	line = NULL;
	while (1)
	{
		status = process_line(write_fd, delimiter, &line);
		if (status == 1)
			return (1);
		if (status == 0)
		{
			free(line);
			break ;
		}
		if (status == -1)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

/* Helper Function for parent() */
static int	handle_child_status(int status, t_exec_context *context)
{
	int	exit_status;

	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		if (exit_status != 0)
		{
			printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted `EOF')\n", 0);
			context->exit_status = exit_status;
			return (-1);
		}
		return (0);
	}
	else if (WIFSIGNALED(status))
	{
		printf("exit signaled quit\n");
		context->exit_status = 128 + WTERMSIG(status);
		return (-1);
	}
	context->exit_status = -1;
	return (-1);
}

/* Refactored child() Function */
int	child(int pipefd[2], const char *delimiter)
{
	int	result;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pipefd[0]);
	result = read_and_write_heredoc(pipefd[1], delimiter);
	close(pipefd[1]);
	if (result == -1)
		exit(1);
	else if (result == 1)
		exit(1);
	exit(0);
}

/* Refactored parent() Function */
int	parent(int pipefd[2], pid_t pid, t_exec_context *context)
{
	int	status;
	int	ret;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	ret = handle_child_status(status, context);
	if (ret == -1)
	{
		close(pipefd[0]);
		return (-1);
	}
	context->stdin_fd = pipefd[0];
	return (0);
}

