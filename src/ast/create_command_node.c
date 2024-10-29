/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:10:28 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/29 15:38:10 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

static void	assign_token_to_argv(t_token **current, char **argv)
{
	int	i;

	i = 0;
	while (*current && is_command_op(*current))
	{
		argv[i] = (*current)->token;
		i++;
		*current = (*current)->next;
	}
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
	t_cmd_node	command_data;
	t_ast_node	*command_node;
	int			argc;
	char		**argv;

	argc = argc_count(current_token);
	argv = (char **)gc_malloc(sizeof(char *) * (argc + 1), gcl);
	gc_lock(argv, gcl);
	assign_token_to_argv(current_token, argv);
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
