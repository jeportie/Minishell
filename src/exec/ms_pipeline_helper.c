/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pipeline_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:57:09 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 17:59:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/process.h"

void	left_child_process(t_pipe_exec_params *params)
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

void	right_child_process(t_pipe_exec_params *params)
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

int	parent_process(pid_t left_pid, pid_t right_pid,
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
