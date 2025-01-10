/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_subshell.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 22:47:38 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 12:43:24 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static t_redir	*st_append_redir_list(t_redir *list1, t_redir *list2)
{
	t_redir	*tmp;

	if (!list1)
		return (list2);
	tmp = list1;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = list2;
	return (list1);
}

static void	st_subshell_child_process(t_subshell_exec_params *params)
{
	t_exec_context	child_context;

	child_context = *params->context;
	child_context.is_subprocess = true;
	child_context.child_lvl = params->context->child_lvl;
	if (child_context.redir_list
		&& ms_apply_redirections(child_context.redir_list) != 0)
		exit(1);
	exit(ms_execute_ast(params->subshell_node->child, &child_context));
}

static int	st_subshell_parent_process(pid_t pid, t_exec_context *context)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		context->shell->error_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		context->shell->error_code = 128 + WTERMSIG(status);
	else
		context->shell->error_code = -1;
	context->redir_list = NULL;
	return (context->shell->error_code);
}

int	ms_execute_subshell(t_subshell_node *subshell_node,
	t_exec_context *context)
{
	pid_t					pid;
	t_subshell_exec_params	params;
	t_redir					*parent_redirs;
	t_redir					*subshell_redirs;
	t_redir					*merged_redirs;

	parent_redirs = context->redir_list;
	subshell_redirs = ms_collect_redirections(subshell_node->child,
			context->shell->gcl, context->shell);
	merged_redirs = st_append_redir_list(parent_redirs, subshell_redirs);
	params.subshell_node = subshell_node;
	params.context = context;
	context->redir_list = merged_redirs;
	pid = fork();
	if (pid == 0)
		st_subshell_child_process(&params);
	return (st_subshell_parent_process(pid, context));
}
