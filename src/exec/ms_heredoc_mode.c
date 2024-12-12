/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_mode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:00:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/12 14:35:52 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	heredoc_child_process(t_exec_context *context, t_heredoc_params *params)
{
	int	result;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	safe_close(params->pipefd[0]);
	result = read_and_write_heredoc(context, params->pipefd[1],
			params->delimiter);
	safe_close(params->pipefd[1]);
	if (result != -1)
		exit(1);
	else if (result == 1)
		exit(1);
	exit(0);
}

int	ms_heredoc_mode(const char *delimiter, t_exec_context *context,
	t_proc_manager *manager, t_gc *gcl)
{
	t_fork_params	fork_params;
	pid_t			pid;

	t_heredoc_params (params) = {delimiter, context, manager, {0}};
	safe_pipe(params.pipefd);
	fork_init(&fork_params, context, true, "heredoc");
	pid = safe_fork(manager, &fork_params);
	if (pid == -1)
	{
		ms_handle_error("minishell: Error: fork failed for heredoc.\n", 0, gcl);
		safe_close(params.pipefd[0]);
		return (safe_close(params.pipefd[1]), -1);
	}
	if (pid == 0)
	{
		context->child_lvl = fork_params.child_lvl;
		heredoc_child_process(context, &params);
	}
	else
		return (heredoc_parent_process(&params, pid));
	return (0);
}
