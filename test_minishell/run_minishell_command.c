/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_minishell_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 22:23:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/25 22:45:30 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

static char	*read_from_fd(int fd)
{
	char	*output;
	size_t	total_size;
	ssize_t	nbytes;
	char	buffer[1024];

	output = NULL;
	total_size = 0;
	while ((nbytes = read(fd, buffer, sizeof(buffer))) > 0)
	{
		char	*new_output = realloc(output, total_size + nbytes + 1);
		if (!new_output)
		{
			free(output);
			return (NULL);
		}
		output = new_output;
		memcpy(output + total_size, buffer, nbytes);
		total_size += nbytes;
	}
	if (output)
		output[total_size] = '\0';
	return (output);
}

int	run_minishell_command(const char *exec_path, const char *command, char **output)
{
	int	in_pipe[2];
	int	out_pipe[2];
	pid_t	pid;
	int	status;
	int	exit_code;
	ssize_t	write_ret;
	size_t	command_len;
	const char	*exit_command = "\nexit\n";
	size_t	exit_command_len;

	exit_code = -1;
	if (!command)
		return (-1);
	if (pipe(in_pipe) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (pipe(out_pipe) == -1)
	{
		perror("pipe");
		close(in_pipe[0]);
		close(in_pipe[1]);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(in_pipe[0]);
		close(in_pipe[1]);
		close(out_pipe[0]);
		close(out_pipe[1]);
		return (-1);
	}
	else if (pid == 0)
	{
		// Child process
		if (dup2(in_pipe[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			_exit(EXIT_FAILURE);
		}
		if (dup2(out_pipe[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			_exit(EXIT_FAILURE);
		}
		if (dup2(out_pipe[1], STDERR_FILENO) == -1)
		{
			perror("dup2");
			_exit(EXIT_FAILURE);
		}
		close(in_pipe[0]);
		close(in_pipe[1]);
		close(out_pipe[0]);
		close(out_pipe[1]);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execl(exec_path, exec_path, NULL);
		perror("execl");
		_exit(EXIT_FAILURE);
	}
	else
	{
		// Parent process
		close(in_pipe[0]);
		close(out_pipe[1]);
		command_len = strlen(command);
		exit_command_len = strlen(exit_command);
		write_ret = write(in_pipe[1], command, command_len);
		if (write_ret == -1 || (size_t)write_ret != command_len)
		{
			perror("write");
			close(in_pipe[1]);
			close(out_pipe[0]);
			waitpid(pid, NULL, 0);
			return (-1);
		}
		write_ret = write(in_pipe[1], "\n", 1);
		if (write_ret == -1 || write_ret != 1)
		{
			perror("write");
			close(in_pipe[1]);
			close(out_pipe[0]);
			waitpid(pid, NULL, 0);
			return (-1);
		}
		write_ret = write(in_pipe[1], exit_command, exit_command_len);
		if (write_ret == -1 || (size_t)write_ret != exit_command_len)
		{
			perror("write");
			close(in_pipe[1]);
			close(out_pipe[0]);
			waitpid(pid, NULL, 0);
			return (-1);
		}
		if (close(in_pipe[1]) == -1)
		{
			perror("close");
			close(out_pipe[0]);
			waitpid(pid, NULL, 0);
			return (-1);
		}
		if (output)
		{
			*output = read_from_fd(out_pipe[0]);
			if (*output == NULL)
			{
				perror("read");
				close(out_pipe[0]);
				waitpid(pid, NULL, 0);
				return (-1);
			}
		}
		else
		{
			char	buffer[1024];
			while (read(out_pipe[0], buffer, sizeof(buffer)) > 0)
				;
		}
		if (close(out_pipe[0]) == -1)
		{
			perror("close");
			waitpid(pid, NULL, 0);
			return (-1);
		}
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			return (-1);
		}
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = 128 + WTERMSIG(status);
		else
			exit_code = -1;
	}
	return (exit_code);
}
