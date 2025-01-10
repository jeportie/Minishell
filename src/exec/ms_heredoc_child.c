/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_child.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:08:10 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 12:45:44 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/expand.h"

static void	st_check_env_var(t_shell *shell, t_here_helper *help)
{
	if (ms_is_var(help->tmp) && ft_strlen(help->tmp) > 1)
	{
		free(help->line);
		help->line = ms_get_env_value(shell->env_data->env, help->tmp + 1,
				shell->error_code);
		if (help->line)
		{
			help->varline = help->line;
			help->line = ft_strjoin_free(help->varline, "\n");
		}
		else
			help->line = ft_strdup("\n");
	}
	else
		help->line = ft_strjoin_free(help->line, "\n");
}

static int	st_process_heredoc_line(t_shell *shell, t_here_helper *help)
{
	if (!help->line)
	{
		ft_dprintf(2, "minishell: warning: here-document at line %d "
			"delimited by end-of-file (wanted `%s')\n",
			help->count_line + 1, help->delim);
		close(help->fd);
		return (0);
	}
	help->count_line++;
	help->tmp = ft_strtrim(help->line, "\n");
	if (!ft_strncmp(help->tmp, help->delim, ft_strlen(help->tmp) + 1))
	{
		free(help->tmp);
		free(help->line);
		close(help->fd);
		return (0);
	}
	st_check_env_var(shell, help);
	free(help->tmp);
	write(help->fd, help->line, ft_strlen(help->line));
	free(help->line);
	return (1);
}

static void	st_init_here_helper(t_here_helper *help, t_shell *shell,
	char *delimiter, char *filename)
{
	help->line = NULL;
	help->delim = delimiter;
	help->tmp = NULL;
	help->varline = NULL;
	help->count_line = 0;
	help->fd = open(filename, O_WRONLY | O_APPEND);
	if (help->fd == -1)
	{
		ft_dprintf(2, "Error: echec open fd.\n");
		rl_clear_history();
		gc_cleanup(shell->gcl);
		exit (1);
	}
}

void	ms_heredoc_child_process(t_shell *shell,
	char *delimiter, char *filename)
{
	t_here_helper	help;

	st_init_here_helper(&help, shell, delimiter, filename);
	signal(SIGINT, ms_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		help.line = readline("\033[1;36m>\033[0m ");
		if (g_signal == 130)
		{
			close(help.fd);
			help.fd = open(filename, O_WRONLY | O_TRUNC);
			if (help.fd == -1)
				ft_dprintf(2, "Error: heredoc open fail.\n");
			else
				close(help.fd);
			exit (130);
		}
		if (!st_process_heredoc_line(shell, &help))
			break ;
	}
	exit(0);
}
