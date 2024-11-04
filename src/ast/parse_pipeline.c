/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:56:14 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 13:18:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

t_ast_node	*parse_pipeline(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*left_node;
	t_ast_node	*right_node;

	left_node = parse_command(current_token, gcl);
	if (!left_node)
		return (NULL);
	while (*current_token && is_pipe_op(*current_token))
	{
		*current_token = (*current_token)->next;
		right_node = parse_command(current_token, gcl);
		if (!right_node)
		{
			ft_dprintf(STDERR, "Minishell: Syntax Error:\
					Expected Command after pipe '|'.\n");
			return (NULL);
		}
		left_node = create_pipe_node(left_node, right_node, gcl);
	}
	return (left_node);
}
