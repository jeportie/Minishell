/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:17:07 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/01 15:17:11 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	ms_execute_command(t_cmd_node *cmd_node, t_exec_context *context, t_gc *gcl)
{
	const char	*cmd;

	cmd = cmd_node->argv[0];
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ms_echo(cmd_node, context));
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (ms_cd(cmd_node, context));
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (ms_pwd(cmd_node, context));
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (ms_export(cmd_node, context));
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (ms_unset(cmd_node, context));
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (ms_env(cmd_node, context));
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (ms_exit(cmd_node, context));
	else
		return (ms_execute_external(cmd_node, context, gcl));
}
