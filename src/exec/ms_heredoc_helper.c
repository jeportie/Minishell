/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:08:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 19:47:20 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	heredoc_parent_process(t_heredoc_params *params, pid_t pid)
{
	int	status;
	int	ret;

	safe_close(params->pipefd[1]);
	waitpid(pid, &status, 0);
	ret = handle_child_status(status, params->context);
	if (ret == -1)
	{
		safe_close(params->pipefd[0]);
		return (-1);
	}
	params->context->stdin_fd = params->pipefd[0];
	return (0);
}

int	process_line(int write_fd, const char *delimiter, char **line_ptr)
{
	size_t	len;

	write(STDOUT_FILENO, "> ", 2);
	*line_ptr = get_next_line(STDIN_FILENO);
	if (!*line_ptr)
		return (1);
	len = ft_strlen(*line_ptr);
	if (len > 0 && (*line_ptr)[len - 1] == '\n')
		(*line_ptr)[len - 1] = '\0';
	if (ft_strncmp(*line_ptr, delimiter, ft_strlen(*line_ptr)) == 0)
		return (0);
	if (write(write_fd, *line_ptr, ft_strlen(*line_ptr)) == -1)
		return (-1);
	if (write(write_fd, "\n", 1) == -1)
		return (-1);
	return (2);
}

int	read_and_write_heredoc(int write_fd, const char *delimiter)
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

int	handle_child_status(int status, t_exec_context *context)
{
	int	exit_status;

	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		if (exit_status != 0)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: warning: heredoc process exited with status %d\n",
				exit_status);
			context->exit_status = exit_status;
			return (-1);
		}
		return (0);
	}
	else if (WIFSIGNALED(status))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: heredoc process terminated by signal %d\n",
			WTERMSIG(status));
		context->exit_status = 128 + WTERMSIG(status);
		return (-1);
	}
	context->exit_status = -1;
	return (-1);
}
