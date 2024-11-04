/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:20:22 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 13:21:33 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

/*void	set_env_value(t_infos *infos, t_env *env, char *name, char *value)
{
	while (env)
	{
		if (ft_strncmp(env->name_folder, name, ft_strlen(name)) == 0)
		{
			env->value_folder = ft_free_str(env->value_folder);
			env->value_folder = ft_strdup(value);
			return ;
		}
		env = env->next;
	}
	t_env (*new_env) = malloc(sizeof(t_env));
	if (!new_env)
		ft_quit(infos, 2, "Error: echec malloc new_env.\n", ENOMEM);
	new_env->name_folder = ft_strdup(name);
	new_env->value_folder = ft_strdup(value);
	value = ft_free_str(value);
	if (!new_env->value_folder || !new_env->name_folder)
		ft_quit(infos, 2, "Error: echec malloc new_env->"
			"value_folder.\n", ENOMEM);
	new_env->next = NULL;
	t_env (*current) = env;
	while (current && current->next != NULL)
		current = current->next;
	if (current)
		current->next = new_env;
}

static int	ft_none_chdir(char *cwd, t_infos *infos)
{
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		pwd = get_env_value(infos->tmp_env, "PWD");
		set_env_value(infos, infos->tmp_env, "OLDPWD", pwd);
		set_env_value(infos, infos->tmp_env, "PWD", cwd);
		cwd = ft_free_str(cwd);
		return (0);
	}
	else
	{
		ft_dprintf(2, "minishell: chdir: error retrieving "
			"current directory: getcwd: cannot access parent "
			"directories: No such file or directory\n", NULL, NULL);
		return (1);
	}
}

int	ft_cd(t_infos *infos, t_tok *tmp)
{
	char *(cd) = NULL;
	char *(cwd) = NULL;
	if (!tmp->cmd[1] || !ft_strncmp(tmp->cmd[1], "~", 2))
	{
		cd = get_env_value(infos->tmp_env, "HOME");
		if (!cd)
			return (ft_dprintf(2, LOG_CD0, NULL, NULL), 1);
	}
	else if (tmp->cmd[2])
		return (ft_dprintf(2, LOG_CD1, NULL, NULL), 1);
	else
	{
		cd = ft_strdup(tmp->cmd[1]);
		if (!cd)
			ft_quit(infos, 2, "Error: echec malloc cd\n", ENOMEM);
	}
	if (chdir(cd))
		ft_dprintf(2, LOG_CD2, tmp->cmd[1], ": No such file or directory\n");
	else
	{
		if (tmp->cmd[1])
			cd = ft_free_str(cd);
		return (ft_none_chdir(cwd, infos));
	}
	return (0);
}*/
