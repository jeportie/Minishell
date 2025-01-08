/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:20:22 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/08 10:02:26 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"
#include "../../include/minishell.h"

static int	st_none_chdir(t_shell *shell)
{
	char	*pwd;
	char	*cwd;
	t_env	*tmp_env;

	tmp_env = shell->env_data->env;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		pwd = ms_get_env_value(tmp_env, "PWD", shell->error_code);
		gc_unlock(pwd, shell->gcl);
		ms_set_env_value(shell, "OLDPWD", pwd);
		ms_set_env_value(shell, "PWD", cwd);
		free (cwd);
		return (0);
	}
	else
	{
		ft_dprintf(2, "%s: chdir: error retrieving "
			"current directory: getcwd: cannot access parent "
			"directories: No such file or directory\n", SHELL);
		return (1);
	}
}

static char	*st_cd_with_option(char *cd, t_shell *shell, char *argv)
{
	if (ft_strlen(argv) == 1)
	{
		if (!ft_strncmp(argv, "~", 2))
		{
			cd = ms_get_env_value(shell->env_data->env, "HOME",
					shell->error_code);
			if (!cd)
				return (ft_dprintf(2, "%s: cd: HOME not set\n", SHELL), NULL);
		}
		else if (!ft_strncmp(argv, "-", 2))
		{
			cd = ms_get_env_value(shell->env_data->env, "OLDPWD",
					shell->error_code);
			if (!cd)
				return (ft_dprintf(2, "%s: cd: OLDPWD not set\n", SHELL), NULL);
			ft_dprintf(1, "%s\n", cd);
		}
	}
	if (!cd)
		cd = gc_strdup(argv, shell->gcl);
	if (!cd)
		echec_malloc(shell->gcl, "cd");
	return (cd);
}

int	ms_cd(t_cmd_node *cmd_node, t_shell *shell)
{
	char *(cd) = NULL;
	if (cmd_node->argc == 1)
	{
		cd = ms_get_env_value(shell->env_data->env, "HOME", shell->error_code);
		if (!cd)
			return (ft_dprintf(2, "%s: cd: HOME not set\n", SHELL), 1);
	}
	else if (cmd_node->argc >= 3)
		return (ft_dprintf(2, "%s: cd: too many arguments\n", SHELL), 1);
	else
	{
		cd = st_cd_with_option(NULL, shell, cmd_node->argv[1]);
		if (!cd)
			return (0);
	}
	if (chdir(cd))
	{
		ft_dprintf(2, "%s: cd: %s: No such file or directory\n",
			SHELL, cmd_node->argv[1]);
		return (1);
	}
	else
		return (st_none_chdir(shell));
	return (0);
}
