/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:01:03 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/29 13:04:55 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

t_ast_node	*parse_subshell(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*subshell_content;
	t_ast_node	*node;

	if (!*current_token || !is_sbs_start(*current_token))
	{
		ft_dprintf(STDERR, "Syntax Error: Expected '('.\n");
		return (NULL);
	}
	*current_token = (*current_token)->next;
	subshell_content = parse_logical(current_token, gcl);
	if (!subshell_content)
		return (NULL);
	if (!*current_token || !is_sbs_stop(*current_token))
	{
		ft_dprintf(STDERR, "Syntax Error: Expected ')'.\n");
		return (NULL);
	}
	*current_token = (*current_token)->next;
	node = create_subshell_node(subshell_content, gcl);
	return (node);
}
