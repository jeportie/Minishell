/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_mode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:00:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/17 12:10:30 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
# include <sys/stat.h>

/*static char	*st_maj_herdoc_line(t_shell *shell, char *line, char *delimiter, int fd)
{
	char *(tmp) = NULL;
	tmp = ft_strtrim(line, "\n");
	if (!ft_strncmp(tmp, delimiter, ft_strlen(tmp) + 1))
	{
		free(tmp);
		free(line);
		close(fd);
		return (NULL);
	}
	if (is_var(tmp) && ft_strlen(tmp) > 1)
	{
		free(line);
		line = ms_get_env_value(shell->env_data->env, tmp + 1, shell->error_code);
		if (line)
		{
			tmp = line;
			line = ft_strjoin_free(tmp, "\n");
		}
		else
			line = ft_strdup("\n");
	}
	return (line);
}

static void	st_heredoc_child_process(t_shell *shell, char *delimiter,
	char *filename)
{
	int (count_line) = 0;
	char *(line) = NULL;
	int	fd = open(filename, O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		ft_dprintf(2, "Error: echec open fd.\n");
		rl_clear_history();
		gc_cleanup(shell->gcl);
		exit (errno);
	}
	signal(SIGINT, ms_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		//ft_dprintf(1, "\033[1;36m>\033[0m ");
		//line = get_next_line(0);
		line = readline("\033[1;36m>\033[0m ");
		count_line++;
		if (line == NULL)
		{
			ft_dprintf(2, "\nminishell: warning: here-document at line %d "
				"delimited by end-of-file (wanted `%s')\n",
				count_line, delimiter);
			free(line);
			close(fd);
			break ;
		}
		line = st_maj_herdoc_line(shell, line, delimiter, fd);
		if (!line)
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	exit(0);
}*/

static void	st_heredoc_child_process(t_shell *shell, char *delimiter, char *filename)
{
	char	*line;
	char	*tmp;

	int count_line = 0;
	int fd = open(filename, O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		ft_dprintf(2, "Error: echec open fd.\n");
		rl_clear_history();
		gc_cleanup(shell->gcl);
		exit (errno);
	}
	signal(SIGINT, ms_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("\033[1;36m>\033[0m ");
		if (g_signal == 130) 
		{
			close(fd);
			fd = open(filename, O_WRONLY | O_TRUNC);
			close(fd);
			exit (130);
		}
		count_line++;
		if (!line)
		{
			ft_dprintf(2, "minishell: warning: here-document at line %d "
					"delimited by end-of-file (wanted `%s')\n",
					count_line + 1, delimiter);
			close(fd);
			break ;
		}
		tmp = ft_strtrim(line, "\n");
		if (!ft_strncmp(tmp, delimiter, ft_strlen(tmp) + 1))
		{
			free(tmp);
			free(line);
			close(fd);
			break ;
		}
		if (is_var(tmp) && ft_strlen(tmp) > 1)
		{
			free(line);
			line = ms_get_env_value(shell->env_data->env, tmp + 1, shell->error_code);
			if (line)
			{
				char *varline = line;
				line = ft_strjoin_free(varline, "\n");
			}
			else
				line = ft_strdup("\n");
		}
		else
			line = ft_strjoin_free(line, "\n");
		free(tmp);
		write(fd, line, ft_strlen(line));
		free(line);
	}
	exit(0);
}

static void	st_fork_heredoc(t_heredoc_node *node, t_exec_context *context)
{
	int	status;

	pid_t (pid) = fork();
	if (pid == 0)
		st_heredoc_child_process(context->shell, node->delimiter, node->filename);
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
		exit (errno);
	}
	if (WIFEXITED(status))
		context->shell->error_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		context->shell->error_code = 128 + WTERMSIG(status);
}

static char	*st_generate_tmp_filename(t_exec_context *context)
{
	char	*base_name = ".ms_tmpfile_";
	char	*file_index;
	char	*filename;
	int		len;

	if (context->tmpfile_counter > 256)
		context->tmpfile_counter = 0;
	file_index = ft_itoa(context->tmpfile_counter++);
	len = ft_strlen(base_name);
	len += ft_strlen(file_index);
	filename = malloc(len + 1);
	ft_memset(filename, 0, len + 1);
	ft_strlcpy(filename, base_name, ft_strlen(base_name) + 1);
	ft_strlcpy(filename + ft_strlen(base_name), file_index, ft_strlen(file_index) + 1);
	if (access(filename, F_OK) == 0)
	{
		gc_unlock(filename, context->shell->gcl);
		return (st_generate_tmp_filename(context));
	}
	else
	{
		int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		close(fd);
		return (filename);
	}
}

int	ms_heredoc_mode(t_heredoc_node *node, t_exec_context *context)
{
	node->filename = st_generate_tmp_filename(context);
	st_fork_heredoc(node, context);
	g_signal = 0;
	return (0);
}
