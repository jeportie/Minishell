/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:07:30 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/14 16:07:34 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/builtins.h"

int	ms_execute_command(t_cmd_node *cmd_node, t_exec_context *context,
	t_proc_manager *manager, t_gc *gcl)
{
	char	*cmd;
	int		i;

	cmd = cmd_node->argv[0];
	i = 1;
	while (i < cmd_node->argc)
	{
		if (is_var(cmd_node->argv[i]))
		{
			cmd_node->argv[i] = ms_expand_arg(cmd_node->argv[i],
					context->shell->env_data->env, gcl);
		}
		i++;
	}
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
