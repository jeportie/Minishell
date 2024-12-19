/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_ast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 09:32:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/18 16:26:24 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/*
static int	redirect_switch(t_ast_node *to_child, t_ast_node *node,
		t_exec_context *context, t_proc_manager *manager)
{
	int (result);
	if (context->redirected == false)
	{
		if (ms_handle_redirections(node, context,
				context->shell->gcl) != 0)
			return (-1);
	}
	result = ms_execute_ast(to_child, context, manager);
	if (context->redirected)
	{
		if (context->stdout_fd != STDOUT_FILENO)
		{
			dup2(context->original_stdout, STDOUT_FILENO);
			close(context->stdout_fd);
			context->stdout_fd = STDOUT_FILENO;
		}
		if (context->stdin_fd != STDIN_FILENO)
		{
			dup2(context->original_stdin, STDIN_FILENO);
			close(context->stdin_fd);
			context->stdin_fd = STDIN_FILENO;
		}
		context->redirected = false;
	}
	return (result);
}
*/
/*
int	ms_execute_ast(t_ast_node *node, t_exec_context *context,
	t_proc_manager *manager)
{
	if (!node)
		return (ms_handle_error("Error: Null AST node.\n", -1,
				context->shell->gcl));
	if (node->type == NODE_COMMAND)
		return (ms_execute_command(&node->data.command, context, manager,
				context->shell->gcl));
	else if (node->type == NODE_PIPE)
		return (ms_execute_pipeline(&node->data.pipe, context, manager));
	else if (node->type == NODE_AND || node->type == NODE_OR)
		return (ms_execute_logical(&node->data.logic, context, node->type,
				manager));
	else if (node->type == NODE_SUBSHELL)
		return (ms_execute_subshell(&node->data.subshell, context, manager));
	else if (node->type == NODE_REDIRECT_IN || node->type == NODE_REDIRECT_OUT
		|| node->type == NODE_REDIRECT_APPEND)
		return (redirect_switch(node->data.redirect.child, node, context,
				manager));
	else if (node->type == NODE_REDIRECT_HEREDOC)
		return (redirect_switch(node->data.heredoc.child, node, context,
				manager));
	else
		return (ms_handle_error("Unsupported node type", -1,
				context->shell->gcl));
}
*/

int	ms_execute_ast(t_ast_node *node, t_exec_context *context,
	t_proc_manager *manager)
{

}
