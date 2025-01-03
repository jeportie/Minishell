/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:31:06 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/02 16:47:21 by jeportie         ###   ########.fr       */
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

void	print_ast_delimit(t_ast_node *root)
{
	int	fd;

	if (DEBUG == 0)
		return ;
	fd = open(PRINT_INFOS, O_WRONLY | O_APPEND, COPY_MODE);
	ft_dprintf(fd, "------------------------------------------------");
	ft_dprintf(fd, "--------------------------------\nAST:\n");
	print_ast(root, 0, "", 0);
	ft_dprintf(fd, "-------------------------------------------"
		"---------------------");
	ft_dprintf(fd, "----------------\n");
	ft_dprintf(fd, "Process Manager - Active Processes:\n");
	ft_dprintf(fd, "Title\t\tPID\tParent PID\tLevel"
		"\tFD_in\tFD_out\tFD_err\tHeredoc\n");
	ft_dprintf(fd, "-------------------------"
		"---------------------------------------");
	ft_dprintf(fd, "----------------\n");
	close(fd);
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
