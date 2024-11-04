/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_ast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 09:32:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 15:37:45 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	ms_execute_ast(t_ast_node *node, t_exec_context *context)
{
	if (node->type == NODE_COMMAND)
		return (ms_execute_command(&node->data.command,
				context, context->shell->gcl));
	else if (node->type == NODE_PIPE)
		return (ms_execute_pipeline(&node->data.pipe, context));
	else if (node->type == NODE_AND || node->type == NODE_OR)
		return (ms_execute_logical(&node->data.logic, context, node->type));
	else if (node->type == NODE_SUBSHELL)
		return (ms_execute_subshell(&node->data.subshell, context));
	else if (node->type == NODE_REDIRECT_IN
		|| node->type == NODE_REDIRECT_OUT
		|| node->type == NODE_REDIRECT_APPEND
		|| node->type == NODE_REDIRECT_HEREDOC)
	{
		if (ms_handle_redirections(node, context) != 0)
			return (-1);
		return (ms_execute_ast((&node->data.redirect)->child, context));
	}
	else
	{
		ms_handle_error("Unsupported node type", context->shell->gcl);
		return (-1);
	}
}
