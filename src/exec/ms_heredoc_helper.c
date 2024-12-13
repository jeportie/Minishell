/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:08:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/12 17:31:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/expand.h"

int	heredoc_parent_process(t_heredoc_params *params, pid_t pid)
{
	int	status;
	int	ret;

	ms_stop_std_signal();
	safe_close(params->pipefd[1]);
	waitpid(pid, &status, 0);
	ret = handle_child_status(status, params);
	if (ret == -1)
	{
		safe_close(params->pipefd[0]);
		return (-1);
	}
	params->context->stdin_fd = params->pipefd[0];
	return (0);
}

int	process_line(t_exec_context *context, int write_fd, const char *delimiter,
		char **line_ptr)
{
	size_t	len;
	char	*tmp;

	write(STDOUT_FILENO, "> ", 2);
	*line_ptr = get_next_line(STDIN_FILENO);
	if (!*line_ptr)
		return (1);
	len = ft_strlen(*line_ptr);
	if (len > 0 && (*line_ptr)[len - 1] == '\n')
		(*line_ptr)[len - 1] = '\0';
	if (ft_strncmp(*line_ptr, delimiter, ft_strlen(*line_ptr)) == 0)
		return (0);
	if (is_var(*line_ptr) && len > 1)
	{
		tmp = ft_strtrim(*line_ptr, " \t\n");
		tmp++;
		*line_ptr = ms_get_env_value(context->shell->env_data->env,
				tmp, context->shell->error_code);
	}
	if (write(write_fd, *line_ptr, ft_strlen(*line_ptr)) == -1)
		return (-1);
	if (write(write_fd, "\n", 1) == -1)
		return (-1);
	return (2);
}

static void	st_eof_in_heredocs(int count_line, const char *delimiter)
{
	ft_dprintf(2, "\nminishell: warning: here-document at line %d "
		"delimited by end-of-file (wanted `%s')\n",
		count_line, delimiter);
}

int	read_and_write_heredoc(t_exec_context *context, int write_fd,
		const char *delimiter)
{
	char	*line;
	int		status;
	int		count_line;

	line = NULL;
	count_line = 0;
	while (1)
	{
		status = process_line(context, write_fd, delimiter, &line);
		count_line++;
		if (status == 1)
			return (st_eof_in_heredocs(count_line, delimiter), 1);
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

int	handle_child_status(int status, t_heredoc_params *params)
{
	int	exit_status;

	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		if (exit_status != 0)
		{
			params->context->exit_status = exit_status;
			return (params->context->exit_status);
		}
		return (0);
	}
	else if (WIFSIGNALED(status))
	{
		ft_dprintf(1, "\n");
		params->context->exit_status = 128 + WTERMSIG(status);
		return (params->context->exit_status);
	}
	params->context->exit_status = -1;
	return (-1);
}

/*int	ft_check_delim(char *line, char *delimiter)
{
	int	i;

	i = 0;
	while (line[i] && delimiter[i])
	{
		if (line[i] == delimiter[i])
			i++;
		else
			return (1);
	}
	if (line[i] == '\n' && !delimiter[i])
		return (0);
	else
		return (1);
}

char	*ft_make_delim(t_infos *infos, char *buff)
{
	int	i;
	int	verif;

	i = 0;
	verif = 0;
	while (buff[i])
	{
		if (ft_isalpha(buff[i]))
			verif++;
		i++;
	}
	if (verif > 0)
		return (ft_strdup(buff));
	else if (!infos->error)
	{
		ft_dprintf(2, LOG_DEL, buff, "\'\n");
		infos->error = "\0";
	}
	return (ft_strdup(buff));
}

void	ft_clear_heredoc_buffer(t_tokenizer *tok)
{
	int	i;

	i = 0;
	tok->i -= ft_strlen(tok->buffer);
	while (i < (int)ft_strlen(tok->buffer))
	{
		tok->buffer[i] = '\0';
		tok->i++;
		i++;
	}
}

void	ft_extract_heredoc_delimiter(t_tokenizer *tok)
{
	tok->j = 0;
	while (tok->input[tok->i] == '<' || tok->input[tok->i] == ' '
		|| tok->input[tok->i] == '>' || tok->input[tok->i] == '|')
		tok->i++;
	while (tok->input[tok->i] && tok->input[tok->i] != ' '
		&& tok->input[tok->i] != '<' && tok->input[tok->i] != '>'
		&& tok->input[tok->i] != '|')
		tok->buffer[tok->j++] = tok->input[tok->i++];
	tok->buffer[tok->j] = '\0';
	tok->i--;
}*/
