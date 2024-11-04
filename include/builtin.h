/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:01:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 16:56:50 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "exec.h"

int	ms_cd(t_cmd_node *cmd_node, t_exec_context *context);
int	ms_echo(t_cmd_node *cmd_node);
int	ms_pwd(void);
int	ms_exit(t_cmd_node *cmd_node, t_exec_context *context);
int	ms_env(t_cmd_node *cmd_node, t_exec_context *context);
int	ms_expand(t_cmd_node *cmd_node, t_exec_context *context);
int	ms_unset(t_cmd_node *cmd_node, t_exec_context *context);

#endif /* BUILTIN_H */
