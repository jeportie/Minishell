/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:31:06 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/18 23:14:30 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

t_ast_node	*ms_parse_tokens(t_token *tokens, t_shell *shell, t_gc *gcl)
{
	t_ast_node	*ast_root;
	t_token		*current_token;

	if (tokens == NULL)
	{
		ft_dprintf(STDERR, "Minishell: Error: No Input provided.\n");
		return (NULL);
	}
	current_token = tokens;
	ast_root = parse_logical(&current_token, shell, gcl);
	if (!ast_root)
	{
		ft_dprintf(STDERR, "Minishell: Error: Failed to parse tokens.\n");
		return (NULL);
	}
	return (ast_root);
}

/*
 * NOTE: Token Priority for binary tree:
 * 
 * Double node tokens: Will create a "branch node"
 *     1- Logic (AND OR)
 *     2- Pipes
 * Simple node tokens: Will create a "leaf node"
 *     1- Subshell ()\
 *     2- Redirections
 *     3- Commands
 *     4- Redirections
 */
