/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_subshell.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 22:47:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 14:37:44 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/process.h"

static void	subshell_child_process(t_subshell_exec_params *params)
{
	t_exec_context	child_context;

	child_context = *(params->context);
	child_context.is_subprocess = true;
	child_context.child_lvl = params->context->child_lvl;
	exit(ms_execute_ast(params->subshell_node->child, &child_context,
			params->manager));
}

static int	subshell_parent_process(pid_t pid, t_exec_context *context)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		context->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		context->exit_status = 128 + WTERMSIG(status);
	else
		context->exit_status = -1;
	return (context->exit_status);
}

int	ms_execute_subshell(t_subshell_node *subshell_node,
	t_exec_context *context, t_proc_manager *manager)
{
	pid_t					pid;
	t_fork_params			fork_params;
	t_subshell_exec_params	params;

	params.subshell_node = subshell_node;
	params.context = context;
	params.manager = manager;
	fork_params.child_lvl = context->child_lvl + 1;
	fork_params.fd_in = context->stdin_fd;
	fork_params.fd_out = context->stdout_fd;
	fork_params.fd_error = context->stderr_fd;
	fork_params.is_heredoc = false;
	fork_params.title = "in subshell process.\n";
	pid = safe_fork(manager, &fork_params);
	if (pid == 0)
	{
		context->child_lvl = fork_params.child_lvl;
		subshell_child_process(&params);
	}
//	print_proc_info(manager);
	return (subshell_parent_process(pid, context));
}
