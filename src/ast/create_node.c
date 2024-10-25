/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:58:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/25 14:30:49 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

t_ast_node	*create_logical_node(t_node_type type, t_ast_node *left,
	t_ast_node *right, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = type;
	node->data.logic.left = left;
	node->data.logic.right = right;
	return (node);
}

t_ast_node	*create_pipe_node(t_ast_node *left, t_ast_node *right, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = NODE_PIPE;
	node->data.pipe.left = left;
	node->data.pipe.right = right;
	return (node);
}

t_ast_node	*create_redirect_node(t_node_type type, t_ast_node *child, char *filename, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = type;
	node->data.redirect.type = type;
	node->data.redirect.child = child;
	node->data.redirect.filename = filename;
	return (node);
}

t_ast_node	*create_command_node(t_token **current_token, t_gc *gcl)
{
	t_cmd_node	command_data;
	t_ast_node	*command_node;
	int			argc;
	char		**argv;
	size_t		argv_size;

	argc = 0;
	argv_size = 4;
	argv = (char **)gc_malloc(sizeof(char *) * argv_size, gcl);
	gc_lock(argv, gcl);
	while (*current_token && ((*current_token)->type == TOKEN_WORD || (*current_token)->type == TOKEN_EXPAND))
	{
		if (argc >= (int)argv_size)
		{
			argv_size *= 2;
			argv = gc_realloc(argv, sizeof(char *) * argv_size, gcl);
			gc_lock(argv, gcl);
		}
		argv[argc] = (*current_token)->token;
		argc++;
		*current_token = (*current_token)->next;
	}
	argv[argc] = NULL;
	command_data.argv = argv;
	command_data.argc = argc;
	command_node = gc_malloc(sizeof(t_ast_node), gcl);
	if (!command_node)
		return (NULL);
	command_node->type = NODE_COMMAND;
	command_node->data.command = command_data;
	return (command_node);
}

t_ast_node	*create_heredoc_node(t_ast_node *child, char *delimiter, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = NODE_REDIRECT_HEREDOC;
	node->data.heredoc.child = child;
	node->data.heredoc.delimiter = delimiter;
	return (node);
}

t_ast_node	*create_subshell_node(t_ast_node *child, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = NODE_SUBSHELL;
	node->data.subshell.child = child;
	return (node);
}
