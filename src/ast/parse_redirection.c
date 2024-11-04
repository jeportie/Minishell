/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:59:50 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 18:00:24 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

static t_node_type	define_type(t_token **current_token)
{
	t_node_type	redir_type;

	if (strcmp((*current_token)->token, "<") == 0)
		redir_type = NODE_REDIRECT_IN;
	else if (strcmp((*current_token)->token, ">") == 0)
		redir_type = NODE_REDIRECT_OUT;
	else if (strcmp((*current_token)->token, ">>") == 0)
		redir_type = NODE_REDIRECT_APPEND;
	else if (strcmp((*current_token)->token, "<<") == 0)
		redir_type = NODE_REDIRECT_HEREDOC;
	else
	{
		ft_dprintf(STDERR, "Syntax Error: Unknown redirection '%s'.\n",
			(*current_token)->token);
		return (ERR);
	}
	return (redir_type);
}

t_ast_node	*parse_redirection(t_token **current_token, t_ast_node *child,
	t_gc *gcl)
{
	t_node_type	redir_type;
	char		*filename;

	while (*current_token && is_redir_op(*current_token))
	{
		redir_type = define_type(current_token);
		if (redir_type == ERR)
			return (NULL);
		*current_token = (*current_token)->next;
		if (!*current_token || !is_command_op(*current_token))
		{
			ft_dprintf(STDERR,
				"Syntax Error: Expected filename after redirection.\n");
			return (NULL);
		}
		filename = (*current_token)->token;
		*current_token = (*current_token)->next;
		if (redir_type == NODE_REDIRECT_HEREDOC)
			child = create_heredoc_node(redir_type, child, filename, gcl);
		else
			child = create_redirect_node(redir_type, child, filename, gcl);
		if (!child)
			return (NULL);
	}
	return (child);
}
