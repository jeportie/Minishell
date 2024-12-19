/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_mode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:00:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/19 10:26:40 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include <sys/stat.h>

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
	char	*filename;
	int		len;
	int		fd;

	char *(base_name) = ".ms_tmpfile_";
	if (context->tmpfile_counter > 256)
		context->tmpfile_counter = 0;
	char *(file_index) = ft_itoa(context->tmpfile_counter++);
	len = ft_strlen(base_name);
	len += ft_strlen(file_index);
	filename = malloc(len + 1);
	ft_memset(filename, 0, len + 1);
	ft_strlcpy(filename, base_name, ft_strlen(base_name) + 1);
	ft_strlcpy(filename + ft_strlen(base_name), file_index,
		ft_strlen(file_index) + 1);
	if (access(filename, F_OK) == 0)
	{
		gc_unlock(filename, context->shell->gcl);
		return (st_generate_tmp_filename(context));
	}
	else
	{
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		return (close(fd), filename);
	}
}

int	ms_heredoc_mode(t_heredoc_node *node, t_exec_context *context)
{
	node->filename = st_generate_tmp_filename(context);
	st_fork_heredoc(node, context);
	g_signal = 0;
	return (0);
}
