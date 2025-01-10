/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:59:01 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 10:17:05 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

static t_ast_node	*ms_in_subshell(t_token **current_token, t_shell *shell,
		t_gc *gcl)
{
	t_ast_node	*node;

	node = ms_parse_subshell(current_token, shell, gcl);
	if (!node)
		return (NULL);
	while (*current_token && ms_is_redir_op(*current_token))
	{
		node = ms_parse_redirection(current_token, node, shell, gcl);
		if (!node)
			return (NULL);
	}
	return (node);
}

t_ast_node	*ms_parse_command(t_token **current_token, t_shell *shell,
	t_gc *gcl)
{
	t_ast_node	*node;

	if (*current_token && ms_is_sbs_start(*current_token))
		return (ms_in_subshell(current_token, shell, gcl));
	if (!*current_token || !ms_is_command_op(*current_token))
	{
		ft_dprintf(STDERR, SHELL ": Syntax Error: Expected a command.\n");
		return (NULL);
	}
	node = ms_create_command_node(current_token, gcl);
	if (!node)
	{
		printf("Node was null\n");
		return (NULL);
	}
	while (*current_token && ms_is_redir_op(*current_token))
	{
		node = ms_parse_redirection(current_token, node, shell, gcl);
		if (!node)
			return (NULL);
	}
	return (node);
}
