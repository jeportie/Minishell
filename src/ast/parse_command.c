/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:59:01 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 13:20:21 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

static t_ast_node	*in_subshell(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*node;

	node = parse_subshell(current_token, gcl);
	if (!node)
		return (NULL);
	while (*current_token && is_redir_op(*current_token))
	{
		node = parse_redirection(current_token, node, gcl);
		if (!node)
			return (NULL);
	}
	return (node);
}

t_ast_node	*parse_command(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*node;

	if (*current_token && is_sbs_start(*current_token))
	{
		return (in_subshell(current_token, gcl));
	}
	if (!*current_token || !is_command_op(*current_token))
	{
		ft_dprintf(STDERR, "Minishell: Syntax Error: Expected a command.\n");
		return (NULL);
	}
	node = create_command_node(current_token, gcl);
	if (!node)
		return (NULL);
	while (*current_token && is_redir_op(*current_token))
	{
		node = parse_redirection(current_token, node, gcl);
		if (!node)
			return (NULL);
	}
	return (node);
}
