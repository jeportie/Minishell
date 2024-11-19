/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:01:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/19 13:32:14 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include "exec.h"

typedef struct s_export_utils
{
	t_shell	*shell;
	int		flag;
	char	*var;
	char	*value;
}				t_export_utils;

void	echec_malloc(t_gc *gcl, char *message);
void	add_cat(t_shell *shell, t_env *current, t_env *tmp, int flag);
void	add_export_utils(t_export_utils *utils, t_env *current, t_env *tmp);
void	add_export(t_export_utils *utils, t_env *ev, char *name_folder,
			char *value_folder);
int		valide_var(char *input);
char	*extract_folder(t_export_utils *utils, char *cmd);
int		ms_export(t_cmd_node *cmd_node, t_exec_context *context);
int		ms_unset(t_cmd_node *cmd_node, t_exec_context *context);
int		ms_cd(t_cmd_node *cmd_node, t_exec_context *context);
int		ms_echo(t_cmd_node *cmd_node);
int		ms_pwd(void);
int		ms_exit(t_cmd_node *cmd_node, t_exec_context *context);
int		ms_env(t_cmd_node *cmd_node, t_exec_context *context);

#endif /* BUILTINS_H */
