/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_mode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:00:00 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/07 13:58:32 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static void	st_fork_heredoc(t_heredoc_node *node, t_exec_context *context)
{
	int	status;

	status = 0;
	pid_t (pid) = fork();
	if (pid == 0)
		st_heredoc_child_process(context->shell, node->delimiter,
			node->filename);
	else if (pid > 0)
	{
		signal(SIGQUIT, st_parent_here_signal);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
	}
	else
	{
		ft_dprintf(2, "Error: echec fork heredoc_buffer.\n");
		rl_clear_history();
		gc_cleanup(context->shell->gcl);
		exit (1);
	}
	if (WIFEXITED(status))
		context->shell->error_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		context->shell->error_code = 128 + WTERMSIG(status);
}

static char	*st_generate_tmp_filename(t_exec_context *context)
{
	int (fd) = 0;
	char *(base_name) = "/tmp/.ms_tmpfile_";
	char *(file_index) = ft_itoa(context->tmpfile_counter++);
	int (len) = ft_strlen(base_name) + ft_strlen(file_index);
	char *(filename) = malloc(len + 1);
	if (context->tmpfile_counter > 256)
		context->tmpfile_counter = 0;
	ft_memset(filename, 0, len + 1);
	ft_strlcpy(filename, base_name, ft_strlen(base_name) + 1);
	ft_strlcpy(filename + ft_strlen(base_name), file_index,
		ft_strlen(file_index) + 1);
	if (access(filename, F_OK) == 0)
		return (gc_unlock(filename, context->shell->gcl),
			st_generate_tmp_filename(context));
	else
	{
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		if (fd == -1)
			return (NULL);
		return (close(fd), filename);
	}
}

int	ms_heredoc_mode(t_heredoc_node *node, t_exec_context *context)
{
	node->filename = st_generate_tmp_filename(context);
	if (!node->filename)
	{
		ft_dprintf(2, "Error: heredoc echec tmpfile generation.\n");
		return (1);
	}
	st_fork_heredoc(node, context);
	g_signal = 0;
	return (0);
}

void	ms_preprocess_heredocs(t_exec_context *context)
{
	int	i;

	i = 0;
	while (i < 16 && context->shell->heredocs[i])
	{
		if (ms_heredoc_mode(context->shell->heredocs[i], context))
		{
			break ;
		}
		i++;
	}
}
