/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:10:28 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/08 12:58:43 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

static bool	st_is_expand(t_token *current_token)
{
	if (current_token->type == TOKEN_EXPAND)
		return (true);
	return (false);
}

static bool	assign_token_to_argv(t_token **current, char **argv)
{
	int		i;
	bool	is_expand;

	i = 0;
	is_expand = false;
	while (*current && is_command_op(*current))
	{
		argv[i] = (*current)->token;
		if (st_is_expand(*current))
			is_expand = true;
		i++;
		*current = (*current)->next;
	}
	return (is_expand);
}

static int	argc_count(t_token **current)
{
	int		i;
	t_token	*head;

	i = 0;
	head = *current;
	while (head && is_command_op(head))
	{
		i++;
		head = head->next;
	}
	return (i);
}

t_ast_node	*create_command_node(t_token **current_token, t_gc *gcl)
{
	t_cmd_node	command;
	t_ast_node	*command_node;
	int			argc;
	char		**argv;

	argc = argc_count(current_token);
	argv = (char **)gc_malloc(sizeof(char *) * (argc + 1), gcl);
	gc_lock(argv, gcl);
	command.is_expand = false;
	if (!assign_token_to_argv(current_token, argv))
		command.is_expand = true;
	argv[argc] = NULL;
	command.argv = argv;
	command.argc = argc;
	command_node = gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(command_node, gcl);
	command_node->type = NODE_COMMAND;
	command_node->data.command = command;
	return (command_node);
}
