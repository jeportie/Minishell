/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:01:00 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 09:27:59 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include "exec.h"

typedef struct s_echo_utils
{
	int	i;
	int	trigger;
	int	flag;
	int	error;
}				t_echo_utils;

typedef struct s_export_utils
{
	t_shell	*shell;
	int		flag;
	char	*var;
	char	*value;
}				t_export_utils;

void	ms_echec_malloc(t_gc *gcl, char *message);
void	ms_add_cat(t_shell *shell, t_env *current, t_env *tmp, int flag);
void	ms_add_export_utils(t_export_utils *utils, t_env **env, t_env *tmp);
void	ms_add_export(t_export_utils *utils, t_env **ev, char *name_folder,
			char *value_folder);
t_env	*ms_copy_env_list(t_env *env, t_gc *gcl);
int		ms_valide_var(char *input);
char	*ms_extract_value(t_export_utils *utils, char *cmd, int size);
char	*ms_extract_folder(t_export_utils *utils, char *cmd);
int		ms_export(t_cmd_node *cmd_node, t_exec_context *context);
int		ms_unset(t_cmd_node *cmd_node, t_exec_context *context);
int		ms_cd(t_cmd_node *cmd_node, t_shell *shell);
int		ms_echo(t_cmd_node *cmd_node, int fd);
int		ms_pwd(int fd);
int		ms_exit(t_cmd_node *cmd_node, t_exec_context *context);
int		ms_env(t_cmd_node *cmd_node, t_exec_context *context);
int		ms_safe_putstr_fd(char *s, int fd);
int		ms_valide_var(char *input);

#endif /* BUILTINS_H */
