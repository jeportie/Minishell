/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_logical.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 22:27:18 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/27 21:04:20 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include <readline/readline.h>

int	ms_execute_logical(t_ast_node *node, t_exec_context *context)
{
	int		left_status;
	bool	run_right;
	int		right_status;

	left_status = ms_execute_ast(node->data.logic.left, context);
	run_right = false;
	if (node->type == NODE_AND && left_status == 0)
		run_right = true;
	else if (node->type == NODE_OR && left_status != 0)
		run_right = true;
	if (run_right)
	{
		right_status = ms_execute_ast(node->data.logic.right, context);
		context->shell->error_code = right_status;
		return (right_status);
	}
	context->shell->error_code = left_status;
	return (left_status);
}
