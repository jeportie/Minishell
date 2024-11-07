/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:34:19 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 19:44:15 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/process.h"

static void	left_child_process(t_pipe_exec_params *params)
{
	safe_close(params->pipefd[0]);
	if (redirect_fd(STDOUT_FILENO, params->pipefd[1]) == -1)
	{
		ms_handle_error("Minishell: Error: redirect_fd failed.\n", 1,
			params->context->shell->gcl);
		exit(EXIT_FAILURE);
	}
	safe_close(params->pipefd[1]);
	exit(ms_execute_ast(params->pipe_node->left, params->context,
			params->manager));
}

static void	right_child_process(t_pipe_exec_params *params)
{
	safe_close(params->pipefd[1]);
	if (redirect_fd(STDIN_FILENO, params->pipefd[0]) == -1)
	{
		ms_handle_error("Minishell: Error: redirect_fd failed.\n", 1,
			params->context->shell->gcl);
		exit(EXIT_FAILURE);
	}
	safe_close(params->pipefd[0]);
	exit(ms_execute_ast(params->pipe_node->right, params->context,
			params->manager));
}

static int	parent_process(pid_t left_pid, pid_t right_pid,
		t_exec_context *context)
{
	int	status;

	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		context->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		context->exit_status = 128 + WTERMSIG(status);
	else
		context->exit_status = -1;
	return (context->exit_status);
}

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

int	ms_execute_pipeline(t_pipe_node *pipe_node, t_exec_context *context,
	t_proc_manager *manager)
{
	t_pipe_exec_params	params;
	pid_t				left_pid;
	pid_t				right_pid;
	t_fork_params		fork_params;

	params.pipe_node = pipe_node;
	params.context = context;
	params.manager = manager;
	safe_pipe(params.pipefd);
	init_left_fork_params(&fork_params, context, params);
	left_pid = safe_fork(manager, &fork_params);
	if (left_pid == 0)
	{
		context->child_lvl = fork_params.child_lvl;
		left_child_process(&params);
	}
	print_proc_info(manager);
	init_right_fork_params(&fork_params, context, params);
	right_pid = safe_fork(manager, &fork_params);
	if (right_pid == 0)
	{
		context->child_lvl = fork_params.child_lvl;
		right_child_process(&params);
	}
	print_proc_info(manager);
	safe_close(params.pipefd[0]);
	safe_close(params.pipefd[1]);
	return (parent_process(left_pid, right_pid, context));
}
