/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_mode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:00:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/12 18:04:11 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/*static void	ft_resize(t_infos *infos, t_tokenizer *tok,
	size_t *new_size, size_t *heredoc_size)
{
	tok->heredoc_buffer = ft_realloc(tok->heredoc_buffer,
			ft_strlen(tok->heredoc_buffer), *new_size);
	if (!tok->heredoc_buffer)
		ft_quit(infos, 2, "Error: echec malloc heredoc_buffer.\n",
			errno);
	*heredoc_size = *new_size;
}

static char	*ft_check_expand(t_infos *infos, char *line)
{
	char	*str;

	str = ft_expand_env_var(infos, line, infos->tmp_env);
	if (str == NULL)
		return (line);
	str = ft_strjoin(str, "\n", '\0', 1);
	return (str);
}

void	heredoc_child_process(t_tokenizer *tok, t_infos *infos,
	char *delimiter, char *filename)
{
	size_t (new_size) = 0;
	char *(line) = NULL;
	size_t (heredoc_size) = BUFFER_SIZE;
	signal(SIGINT, ft_heredoc_signal);
	while (1 && !infos->error)
	{
		ft_dprintf(1, "\033[1;36m>\033[0m ", NULL, NULL);
		line = get_next_line(0, '\n');
		if (line && line[0] == '$')
			line = ft_check_expand(infos, line);
		if (line == NULL || !ft_check_delim(line, delimiter))
		{
			if (line == NULL)
				ft_dprintf(2, LOG_ERR, delimiter, "\')\n");
			line = ft_free_str(line);
			break ;
		}
		new_size = ft_strlen(tok->heredoc_buffer) + ft_strlen(line) + 2;
		if (new_size > heredoc_size)
			ft_resize(infos, tok, &new_size, &heredoc_size);
		ft_strcat(tok->heredoc_buffer, line);
		line = ft_free_str(line);
	}
	ft_generate(infos, tok, filename, delimiter);
	exit(0);
}

void	ft_heredoc(t_tokenizer *tok, t_infos *infos, char *delimiter,
	char *tmp_filename)
{
	int	status;

	pid_t (pid) = fork();
	if (pid == 0)
		heredoc_child_process(tok, infos, delimiter, tmp_filename);
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		ft_quit(infos, 2, "Error: echec fork heredoc_buffer.\n", errno);
}*/

int	ms_heredoc_mode(const char *delimiter, t_exec_context *context,
	t_proc_manager *manager, t_gc *gcl)
{
//	char	*delim;
	char	tmp_filename[260];

	if (gcl && delimiter && context && manager)
		ft_generate_temp_filename(tmp_filename, sizeof(tmp_filename),
			context);
	printf("filename = %s\n", tmp_filename);
	return (0);
}

/*void	ft_handle_heredoc(t_tokenizer *tok, t_infos *infos)
{
	char	*delimiter;
	char	tmp_filename[260];

	ft_generate_temp_filename(tmp_filename, sizeof(tmp_filename),
		infos->tmpfile_counter);
	delimiter = ft_make_delim(infos, tok->buffer);
	tok->heredoc_buffer = malloc(BUFFER_SIZE);
	if (!tok->heredoc_buffer)
		ft_quit(infos, 2, "Error: echec malloc heredoc_buffer.\n", errno);
	tok->heredoc_buffer[0] = '\0';
	g_signal = 1;
	ft_heredoc(tok, infos, delimiter, tmp_filename);
	if (g_signal == 1)
	{
		ft_add_token(infos, &infos->tokens, TOKEN_HEREDOC_WORD, tmp_filename);
		g_signal = 0;
	}
	ft_add_token(infos, &infos->tokens, TOKEN_HEREDOC_DELIMITER, delimiter);
	if (infos->tmpfile_counter == 256)
		infos->tmpfile_counter = 0;
	else
		infos->tmpfile_counter++;
	delimiter = ft_free_str(delimiter);
	tok->heredoc_buffer = ft_free_str(tok->heredoc_buffer);
}*/
