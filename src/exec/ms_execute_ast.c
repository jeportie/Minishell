/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_ast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 09:32:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 14:00:09 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static int	redirect_switch(t_ast_node *to_child, t_ast_node *node,
		t_exec_context *context)
{
	if (ms_handle_redirections(node, context, context->shell->gcl) != 0)
		return (-1);
	return (ms_execute_ast(to_child, context));
}

int	ms_execute_ast(t_ast_node *node, t_exec_context *context)
{
	if (!node)
		return (ms_handle_error("Error: Null AST node.\n", -1,
				context->shell->gcl));
	if (node->type == NODE_COMMAND)
		return (ms_execute_command(&node->data.command, context,
				context->shell->gcl));
	else if (node->type == NODE_PIPE)
		return (ms_execute_pipeline(&node->data.pipe, context));
	else if (node->type == NODE_AND || node->type == NODE_OR)
		return (ms_execute_logical(&node->data.logic, context, node->type));
	else if (node->type == NODE_SUBSHELL)
		return (ms_execute_subshell(&node->data.subshell, context));
	else if (node->type == NODE_REDIRECT_IN || node->type == NODE_REDIRECT_OUT
		|| node->type == NODE_REDIRECT_APPEND)
		return (redirect_switch(node->data.redirect.child, node, context));
	else if (node->type == NODE_REDIRECT_HEREDOC)
		return (redirect_switch(node->data.heredoc.child, node, context));
	else
		return (ms_handle_error("Unsupported node type", -1,
				context->shell->gcl));
}
