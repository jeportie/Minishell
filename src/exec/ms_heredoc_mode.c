/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_mode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:00:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/05 18:10:24 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/* Final ms_heredoc_mode() Function */
int	ms_heredoc_mode(const char *delimiter, t_exec_context *context, t_gc *gcl)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		ms_handle_error("Minishell: Error: pipe failed for heredoc.\n", gcl);
		return (-1);
	}
	pid = fork();
	if (pid < 0)
	{
		ms_handle_error("Minishell: Error: fork failed for heredoc.\n", gcl);
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		return (child(pipefd, delimiter));
	else
		return (parent(pipefd, pid, context));
}
