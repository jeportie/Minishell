/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:56:14 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 10:26:44 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

t_ast_node	*ms_parse_pipeline(t_token **current_token, t_shell *shell,
	t_gc *gcl)
{
	t_ast_node	*left_node;
	t_ast_node	*right_node;

	left_node = ms_parse_command(current_token, shell, gcl);
	if (!left_node)
		return (NULL);
	while (*current_token && ms_is_pipe_op(*current_token))
	{
		*current_token = (*current_token)->next;
		right_node = ms_parse_command(current_token, shell, gcl);
		if (!right_node)
		{
			ft_dprintf(STDERR, SHELL ": Syntax Error:\
					Expected Command after pipe '|'.\n");
			return (NULL);
		}
		left_node = ms_create_pipe_node(left_node, right_node, gcl);
	}
	return (left_node);
}
