/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:34:19 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/03 17:27:13 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static void	init_context(t_pipe_context *pipe_context, t_exec_context *context)
{
	(*pipe_context).context = context;
	(*pipe_context).last_pid = -1;
	(*pipe_context).last_status = 0;
}

static void	child_process(t_pipe_context *pipe_context, t_pipe_node *pipe_node,
		t_exec_context *context, bool is_left)
{
	t_gc	*child_gcl;

	child_gcl = gc_init();
	if (is_left)
	{
		if (dup2(pipe_context->pipefd[1], STDOUT_FILENO) == -1)
		{
			ms_handle_error("Minishell: Error: dup2 failed.\n", child_gcl);
			exit(EXIT_FAILURE);
		}
		close(pipe_context->pipefd[0]);
		close(pipe_context->pipefd[1]);
		exit(ms_execute_ast(pipe_node->left, context));
	}
	else
	{
		if (dup2(pipe_context->pipefd[0], STDIN_FILENO) == -1)
		{
			ms_handle_error("Minishell: Error: dup2 failed.\n", child_gcl);
			exit(EXIT_FAILURE);
		}
		close(pipe_context->pipefd[1]);
		close(pipe_context->pipefd[0]);
		exit(ms_execute_ast(pipe_node->right, context));
	}
}

static void	parent_process(t_pipe_context *pipe_context,
		t_exec_context *context, pid_t left_pid, pid_t right_pid)
{
	close(pipe_context->pipefd[0]);
	close(pipe_context->pipefd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &pipe_context->last_status, 0);
	if (WIFEXITED(pipe_context->last_status))
		context->exit_status = WEXITSTATUS(pipe_context->last_status);
	else if (WIFSIGNALED(pipe_context->last_status))
		context->exit_status = 128 + WTERMSIG(pipe_context->last_status);
	else
		context->exit_status = -1;
}

int	ms_execute_pipeline(t_pipe_node *pipe_node, t_exec_context *context)
{
	t_pipe_context	pipe_context;
	pid_t			left_pid;
	pid_t			right_pid;

	init_context(&pipe_context, context);
	if (pipe(pipe_context.pipefd) == -1)
		return (ms_handle_error("Minishell: Error: pipe failed\n", NULL));
	left_pid = fork();
	if (left_pid < 0)
		return (ms_handle_error("Minishell: Error: fork failed\n", NULL));
	if (left_pid == 0)
		child_process(&pipe_context, pipe_node, context, true);
	right_pid = fork();
	if (right_pid < 0)
		return (ms_handle_error("Minishell: Error: fork failed\n", NULL));
	if (right_pid == 0)
		child_process(&pipe_context, pipe_node, context, false);
	parent_process(&pipe_context, context, left_pid, right_pid);
	return (context->exit_status);
}
