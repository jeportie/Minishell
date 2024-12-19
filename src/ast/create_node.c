/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:58:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/18 23:10:39 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"

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

t_ast_node	*create_redirect_node(t_node_type type, t_ast_node *child,
	char *filename, t_gc *gcl)
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

t_ast_node	*create_heredoc_node(t_node_type type, t_ast_node *child,
	char *delimiter, t_gc *gcl)
{
	t_ast_node	*node;
	int			i;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = type;
	node->data.heredoc.type = 2;
	node->data.heredoc.child = child;
	node->data.heredoc.delimiter = delimiter;
	node->data.heredoc.filename = NULL;
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
