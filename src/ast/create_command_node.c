/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:10:28 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/29 13:25:58 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

static int	helper(t_token **current, size_t argv_size, char **argv, t_gc *gcl)
{
	int	argc;

	argc = 0;
	while (*current && is_command_op(*current))
	{
		if (argc >= (int)argv_size)
		{
			argv_size *= 2;
			argv = gc_realloc(argv, sizeof(char *) * argv_size, gcl);
			gc_lock(argv, gcl);
		}
		argv[argc] = (*current)->token;
		argc++;
		*current = (*current)->next;
	}
	return (argc);
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
	argc = helper(current_token, argv_size, argv, gcl);
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
