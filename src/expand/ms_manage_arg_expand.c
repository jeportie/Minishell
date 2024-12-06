/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_manage_arg_expand.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:12:43 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/19 15:38:15 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/expand.h"

static	void	expand_argv(char **new_argv, char **argv,
		t_wildcard_context *match_ctx)
{
	int		argv_index;
	int		new_argv_index;
	int		match_index;

	argv_index = 0;
	new_argv_index = 0;
	match_index = 0;
	while (argv[argv_index])
	{
		if (argv[argv_index][0] == '*')
		{
			while (match_index < (int)match_ctx->match_count)
			{
				new_argv[new_argv_index] = match_ctx->matches[match_index];
				new_argv_index++;
				match_index++;
			}
		}
		else
			new_argv[new_argv_index] = argv[argv_index];
		argv_index++;
		new_argv_index++;
	}
	new_argv[new_argv_index] = NULL;
}

char	**cmd_arg_expansion(char **argv, t_wildcard_context *match_ctx,
			t_gc *gcl)
{
	char	**new_argv;
	int		i;
	int		total_args;

	i = 0;
	total_args = (int)match_ctx->match_count;
	while (argv[i])
	{
		i++;
		total_args++;
	}
	new_argv = (char **)gc_malloc(sizeof(char *) * total_args, gcl);
	ft_memset(new_argv, 0, sizeof(char *) * total_args);
	expand_argv(new_argv, argv, match_ctx);
	return (new_argv);
}

void	ms_manage_arg_expand(t_cmd_node *cmd_node,
			t_exec_context *context, t_gc *gcl)
{
	int					i;
	t_wildcard_context	*matches;

	i = 0;
	if (!is_equal(cmd_node->argv[0]))
	{
		while (i < cmd_node->argc)
		{
			if (is_var(cmd_node->argv[i]))
			{
				cmd_node->argv[i] = nested_vars(cmd_node->argv[i],
						context->shell);
			}
			if (is_wild(cmd_node->argv[i]))
			{
				matches = ms_expand_wild(cmd_node->argv[i], gcl);
				//printf("-> %s\n", matches->matches[0]);
				cmd_node->argc += matches->match_count - 1;
				cmd_node->argv = cmd_arg_expansion(cmd_node->argv,
						matches, gcl);
			}
			i++;
		}
	}
}
