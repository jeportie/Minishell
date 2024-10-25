/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:00:08 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/25 14:30:05 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

t_ast_node	*parse_subshell(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*subshell_content;
	t_ast_node	*node;

	if (!*current_token || (*current_token)->type != TOKEN_SUBSHELL_START)
	{
        ft_dprintf(STDERR, "Syntax Error: Expected '('.\n");
        return NULL;
	}
	*current_token = (*current_token)->next;
	subshell_content = parse_logical(current_token, gcl);
	if (!subshell_content)
		return (NULL);
	if (!*current_token || (*current_token)->type != TOKEN_SUBSHELL_STOP)
	{
        ft_dprintf(STDERR, "Syntax Error: Expected ')'.\n");
        return NULL;
	}
	*current_token = (*current_token)->next;
	node = create_subshell_node(subshell_content, gcl);
	return (node);
}

t_ast_node *parse_redirection(t_token **current_token, t_ast_node *child, t_gc *gcl)
{
	t_node_type	redir_type;
	char		*filename;

	while (*current_token && (*current_token)->type == TOKEN_REDIRECTION)
	{
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
			ft_dprintf(STDERR, "Syntax Error: Unknown redirection '%s'.\n", (*current_token)->token);
			return (NULL);
		}
		*current_token = (*current_token)->next;
		if (!*current_token || (*current_token)->type != TOKEN_WORD)
		{
			ft_dprintf(STDERR, "Syntax Error: Expected filename after redirection.\n");
			return (NULL);
		}
		filename = (*current_token)->token;
		*current_token = (*current_token)->next;
		if (redir_type == NODE_REDIRECT_HEREDOC)
			child = create_heredoc_node(child, filename, gcl);
		else
			child = create_redirect_node(redir_type, child, filename, gcl);
		if (!child)
			return (NULL);
	}
	return (child);
}

t_ast_node	*parse_command(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*node;

	if (*current_token && (*current_token)->type == TOKEN_SUBSHELL_START)
	{
		node = parse_subshell(current_token, gcl);
		if (!node)
			return (NULL);
		while (*current_token && (*current_token)->type == TOKEN_REDIRECTION)
		{
			node = parse_redirection(current_token, node, gcl);
			if (!node)
				return (NULL);
		}
		return (node);
	}
	if (!*current_token || (*current_token)->type != TOKEN_WORD)
	{
		ft_dprintf(STDERR, "Minishell: Syntax Error: Expected a command.\n");
		return (NULL);
	}
	node = create_command_node(current_token, gcl);
	if (!node)
		return (NULL);
	while (*current_token && (*current_token)->type == TOKEN_REDIRECTION)
	{
		node = parse_redirection(current_token, node, gcl);
		if (!node)
			return (NULL);
	}
	return (node);
}

t_ast_node	*parse_logical(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*left_node;
	t_ast_node	*right_node;
	t_node_type	op_type;

	left_node = parse_pipeline(current_token, gcl);
	if (!left_node)
		return (NULL);
	while (*current_token && is_logical_op(*current_token))
	{
		if ((*current_token)->type == TOKEN_AND)
			op_type = NODE_AND;
		else if ((*current_token)->type == TOKEN_OR)
			op_type = NODE_OR;
		else
			return (left_node);
		*current_token = (*current_token)->next;
		right_node = parse_pipeline(current_token, gcl);
		if (!right_node)
		{
			ft_dprintf(STDERR, "Minishell: Syntax Error: Expected Command after logical operator.\n");
			return (NULL);
		}
		left_node = create_logical_node(op_type, left_node, right_node, gcl);
	}
	return (left_node);
}

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
			ft_dprintf(STDERR, "Minishell: Syntax Error: Expected Command after pipe '|'.\n");
			return (NULL);
		}
		left_node = create_pipe_node(left_node, right_node, gcl);
	}
	return (left_node);
}
