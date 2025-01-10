/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_subshell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:01:03 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 10:34:28 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

t_ast_node	*ms_parse_subshell(t_token **current_token, t_shell *shell,
	t_gc *gcl)
{
	t_ast_node	*subshell_content;
	t_ast_node	*node;

	if (!*current_token || !ms_is_sbs_start(*current_token))
	{
		ft_dprintf(STDERR, "Syntax Error: Expected '('.\n");
		return (NULL);
	}
	*current_token = (*current_token)->next;
	subshell_content = ms_parse_logical(current_token, shell, gcl);
	if (!subshell_content)
		return (NULL);
	if (!*current_token || !ms_is_sbs_stop(*current_token))
	{
		ft_dprintf(STDERR, "Syntax Error: Expected ')'.\n");
		return (NULL);
	}
	*current_token = (*current_token)->next;
	node = ms_create_subshell_node(subshell_content, gcl);
	return (node);
}
