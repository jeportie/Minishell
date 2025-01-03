/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:59:50 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/03 19:14:18 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"
#include <readline/readline.h>

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

static t_ast_node	*st_redirect_helper(t_ast_node *child,
		t_node_type redir_type, t_shell *shell, char *filename)
{
	int			i;

	child = create_heredoc_node(redir_type, child, filename, shell->gcl);
	i = 0;
	while (shell->heredocs[i])
		i++;
	if (i > 16)
	{
		ft_dprintf(STDERR,
			"bash: maximum here-document count exceeded\n");
		rl_clear_history();
		gc_cleanup(shell->gcl);
		exit(2);
	}
	shell->heredocs[i] = &((t_ast_node *)child)->data.heredoc;
	return (child);
}

t_ast_node	*parse_redirection(t_token **current_token, t_ast_node *child,
	t_shell *shell, t_gc *gcl)
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
			ft_dprintf(STDERR, "Error: Expected filename after redirection.\n");
			return (NULL);
		}
		filename = (*current_token)->token;
		*current_token = (*current_token)->next;
		if (redir_type == NODE_REDIRECT_HEREDOC)
			child = st_redirect_helper(child, redir_type, shell, filename);
		else
			child = create_redirect_node(redir_type, child, filename, gcl);
		if (!child)
			return (NULL);
	}
	return (child);
}
