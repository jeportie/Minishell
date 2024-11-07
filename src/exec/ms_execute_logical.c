/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_logical.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 22:27:18 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 16:48:31 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include <readline/readline.h>

int	ms_execute_logical(t_logic_node *logic_node, t_exec_context *context,
		t_node_type type, t_proc_manager *manager)
{
	int	left_exit_status;
	int	right_exit_status;

	right_exit_status = 0;
	left_exit_status = ms_execute_ast(logic_node->left, context, manager);
	if ((type == NODE_AND && left_exit_status == 0)
		|| (type == NODE_OR && left_exit_status))
	{
		right_exit_status = ms_execute_ast(logic_node->right, context, manager);
		context->exit_status = right_exit_status;
	}
	else
		context->exit_status = left_exit_status;
	return (context->exit_status);
}
