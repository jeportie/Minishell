/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:07:30 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/18 23:50:35 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/builtins.h"
#include <unistd.h>

char	**cmd_arg_expansion(char **argv, t_wildcard_context *match_ctx, t_gc *gcl)
{
	char	**new_argv;
	int		i;
	int		j;
	int		h;
	int		total_args;

	i = 0;
	total_args = (int)match_ctx->match_count;
	while (argv[i])
	{
		i++;
		total_args++;
	}
	new_argv = (char **)gc_malloc(sizeof(char *) * total_args, gcl);
	i = 0;
	j = 0;
	h = 0;
	while (argv[i])
	{
		if (argv[i][0] == '*')
		{
			while (h < (int)match_ctx->match_count)
			{
				new_argv[j] = match_ctx->matches[h];
				j++;
				h++;
			}
		}
		else
			new_argv[j] = argv[i];
		i++;
		j++;
	}
	new_argv[j] = NULL;
	return (new_argv);
}

int	ms_execute_command(t_cmd_node *cmd_node, t_exec_context *context,
	t_proc_manager *manager, t_gc *gcl)
{
	t_wildcard_context	*matches;
	char				*cmd;
	int					i;

	i = 0;
	if (!is_equal(cmd_node->argv[0]))
	{
		while (i < cmd_node->argc)
		{
			if (is_var(cmd_node->argv[i]))
			{
				cmd_node->argv[i] = nested_vars(cmd_node->argv[i],
						context->shell->env_data->env, gcl);
			}
			if (is_wild(cmd_node->argv[i]))
			{
				matches = ms_expand_wild(cmd_node->argv[i], gcl);
				cmd_node->argc += matches->match_count - 1;
				cmd_node->argv = cmd_arg_expansion(cmd_node->argv, matches, gcl);
			}
			i++;
		}
	}
	cmd = cmd_node->argv[0];
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (ms_echo(cmd_node));
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (ms_cd(cmd_node, context));
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (ms_pwd());
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (ms_export(cmd_node, context));
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (ms_unset(cmd_node, context));
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (ms_env(cmd_node, context));
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (ms_exit(cmd_node, context));
	else if (is_equal(cmd))
		return (ms_value_assign(context->shell, cmd_node, gcl));
	else
		return (ms_execute_external(cmd_node, context, manager, gcl));
}
