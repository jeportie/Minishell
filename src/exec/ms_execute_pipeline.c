/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:34:19 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/20 15:24:18 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/process.h"

static	void	init_left_fork_params(t_fork_params *fork_params,
	t_exec_context *context, t_pipe_exec_params params)
{
	fork_params->child_lvl = context->child_lvl + 1;
	fork_params->fd_in = context->stdin_fd;
	fork_params->fd_out = params.pipefd[1];
	fork_params->fd_error = context->stderr_fd;
	fork_params->is_heredoc = false;
	fork_params->title = "L-Pipe";
}

static	void	init_right_fork_params(t_fork_params *fork_params,
	t_exec_context *context, t_pipe_exec_params params)
{
	fork_params->child_lvl = context->child_lvl + 1;
	fork_params->fd_in = params.pipefd[0];
	fork_params->fd_out = context->stdout_fd;
	fork_params->fd_error = context->stderr_fd;
	fork_params->is_heredoc = false;
	fork_params->title = "R-Pipe";
}

void	init_helper(t_pipe_exec_params *params, t_pipe_node *pipe_node,
			t_exec_context *context, t_proc_manager *manager)
{
	params->pipe_node = pipe_node;
	params->context = context;
	params->manager = manager;
	safe_pipe(params->pipefd);
}

int	ms_execute_pipeline(t_pipe_node *pipe_node, t_exec_context *context,
	t_proc_manager *manager)
{
	t_pipe_exec_params	params;
	pid_t				left_pid;
	pid_t				right_pid;
	t_fork_params		fork_params;

	init_helper(&params, pipe_node, context, manager);
	init_left_fork_params(&fork_params, context, params);
	left_pid = safe_fork(manager, &fork_params);
	if (left_pid == 0)
	{
		context->child_lvl = fork_params.child_lvl;
		left_child_process(&params);
	}
//	print_proc_info(manager);
	init_right_fork_params(&fork_params, context, params);
	right_pid = safe_fork(manager, &fork_params);
	if (right_pid == 0)
	{
		context->child_lvl = fork_params.child_lvl;
		right_child_process(&params);
	}
//	print_proc_info(manager);
	safe_close(params.pipefd[0]);
	safe_close(params.pipefd[1]);
	return (parent_process(left_pid, right_pid, context));
}
