/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:17:07 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 17:20:50 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/*
 * NOTE:
 * -Retourner dans creation token word et node command et ajouter une focntion
 *  pour expand les variables $
 * -Verfier que command est expand ou pas
 *  -Si expand var ou wild card, appliquer les focntions sur l'argv en question.
 */

/*
 * NOTE:
 * Gerer retour erreur des expand * $
 */

int	ms_execute_command(t_cmd_node *cmd_node, t_exec_context *context, t_gc *gcl)
{
//	const char	*cmd;

//	cmd = cmd_node->argv[0];
//	if (ft_strncmp(cmd, "echo", 5) == 0)
//		return (ms_echo(cmd_node));
//	else if (ft_strncmp(cmd, "cd", 3) == 0)
//		return (ms_cd(cmd_node, context));
//	else if (ft_strncmp(cmd, "pwd", 4) == 0)
//		return (ms_pwd());
//	else if (ft_strncmp(cmd, "export", 7) == 0)
//		return (ms_export(cmd_node, context));
//	else if (ft_strncmp(cmd, "unset", 6) == 0)
//		return (ms_unset(cmd_node, context));
//	else if (ft_strncmp(cmd, "env", 4) == 0)
//		return (ms_env(cmd_node, context));
//	else if (ft_strncmp(cmd, "exit", 5) == 0)
//		return (ms_exit(cmd_node, context));
//	else
		return (ms_execute_external(cmd_node, context, gcl));
}
