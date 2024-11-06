/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_subshell.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 22:47:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 10:32:01 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include <sys/wait.h>

int	ms_execute_subshell(t_subshell_node *subshell_node, t_exec_context *context)
{
	pid_t			pid;
	int				status;
	t_exec_context	child_context;

	child_context = *context;
	pid = fork();
	if (pid < 0)
		return (ms_handle_error("Minishell: Error: fork failed\n", 1, NULL));
	if (pid == 0)
	{
		child_context.is_subprocess = true;
		exit (ms_execute_ast(subshell_node->child, &child_context));
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			context->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			context->exit_status = 128 + WTERMSIG(status);
		else
			context->exit_status = -1;
		return (context->exit_status);
	}
}
