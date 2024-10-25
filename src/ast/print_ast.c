/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:17:41 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/25 15:35:51 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

t_ast_node	*get_left_child(t_ast_node *node)
{
	if (node == NULL)
		return (NULL);
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (node->data.logic.left);
	else if (node->type == NODE_PIPE)
		return (node->data.pipe.left);
	else if (node->type == NODE_REDIRECT_OUT || node->type == NODE_REDIRECT_IN
		|| node->type == NODE_REDIRECT_APPEND
		|| node->type == NODE_REDIRECT_HEREDOC)
		return (node->data.redirect.child);
	else if (node->type == NODE_SUBSHELL)
		return (node->data.subshell.child);
	else
		return (NULL);
}

t_ast_node	*get_right_child(t_ast_node *node)
{
	if (node == NULL)
		return (NULL);
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (node->data.logic.right);
	else if (node->type == NODE_PIPE)
		return (node->data.pipe.right);
	else
		return (NULL);
}

char	*get_node_label(t_ast_node *node)
{
	if (node == NULL)
		return ("");
	if (node->type == NODE_COMMAND)
		return ("\033[1;32m[CMD]\033[0m");
	else if (node->type == NODE_AND)
		return ("\033[1;34m{&&}\033[0m");
	else if (node->type == NODE_OR)
		return ("\033[1;34m{||}\033[0m");
	else if (node->type == NODE_PIPE)
		return ("\033[1;34m{|}\033[0m");
	else if (node->type == NODE_REDIRECT_OUT)
		return ("\033[1;34m{>}\033[0m");
	else if (node->type == NODE_REDIRECT_IN)
		return ("\033[1;34m{<}\033[0m");
	else if (node->type == NODE_REDIRECT_APPEND)
		return ("\033[1;34m{>>}\033[0m");
	else if (node->type == NODE_REDIRECT_HEREDOC)
		return ("\033[1;34m{<<}\033[0m");
	else if (node->type == NODE_SUBSHELL)
		return ("\033[1;34m{( )}\033[0m");
	else
		return ("[UNKNOWN]");
}

void	print_node_content(t_ast_node *node)
{
	int			i;
	t_cmd_node	cmd;

	if (node == NULL)
		return ;
	if (node->type == NODE_COMMAND)
	{
		cmd = node->data.command;
		printf(" (");
		i = 0;
		while (i < cmd.argc)
		{
			printf("%s", cmd.argv[i]);
			if (i < cmd.argc - 1)
				printf(", ");
			i++;
		}
		printf(")");
	}
	else if (node->type == NODE_REDIRECT_OUT || node->type == NODE_REDIRECT_IN
		|| node->type == NODE_REDIRECT_APPEND)
		printf(" (File: %s)", node->data.redirect.filename);
	else if (node->type == NODE_REDIRECT_HEREDOC)
		printf(" (Delimiter: %s)", node->data.heredoc.delimiter);
}

void	print_ast(t_ast_node *node, int depth, char *prefix, int is_left)
{
	char	new_prefix[256];

	if (node == NULL)
		return ;
	printf("%s", prefix);
	if (depth > 0)
	{
		if (is_left)
			printf("├── ");
		else
			printf("└── ");
	}
	else
		printf("└── ");
	printf("%s", get_node_label(node));
	print_node_content(node);
	printf("\n");
	if (is_left)
		snprintf(new_prefix, sizeof(new_prefix), "%s│   ", prefix);
	else
		snprintf(new_prefix, sizeof(new_prefix), "%s    ", prefix);
	print_ast(get_left_child(node), depth + 1, new_prefix, 1);
	print_ast(get_right_child(node), depth + 1, new_prefix, 0);
}

t_ast_node	*create_command_node_with_args(int argc, ...)
{
	t_ast_node	*node;
	va_list		args;
	int			i;
	char		*arg;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->data.command.argc = argc;
	node->data.command.argv = malloc(sizeof(char *) * (argc + 1));
	va_start(args, argc);
	i = 0;
	while (i < argc)
	{
		arg = va_arg(args, char *);
		node->data.command.argv[i] = arg;
		i++;
	}
	va_end(args);
	node->data.command.argv[argc] = NULL;
	return (node);
}

