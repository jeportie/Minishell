/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_logical.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:54:01 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 10:25:22 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

t_ast_node	*ms_parse_logical(t_token **current_token, t_shell *shell,
	t_gc *gcl)
{
	t_ast_node	*left_node;
	t_ast_node	*right_node;
	t_node_type	op_type;

	left_node = ms_parse_pipeline(current_token, shell, gcl);
	if (!left_node)
		return (NULL);
	while (*current_token && ms_is_logical_op(*current_token))
	{
		if ((*current_token)->type == TOKEN_AND)
			op_type = NODE_AND;
		else if ((*current_token)->type == TOKEN_OR)
			op_type = NODE_OR;
		else
			return (left_node);
		*current_token = (*current_token)->next;
		right_node = ms_parse_pipeline(current_token, shell, gcl);
		if (!right_node)
		{
			ft_dprintf(STDERR, SHELL ": Syntax Error:\
					Expected Command after logical operator.\n");
			return (NULL);
		}
		left_node = ms_create_logical_node(op_type, left_node, right_node, gcl);
	}
	return (left_node);
}