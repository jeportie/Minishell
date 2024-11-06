/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:20:22 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 17:04:50 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static char	*st_get_env_value(t_env *env, char *name)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->var, name, ft_strlen(name) + 1) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

static void	st_set_env_value(t_exec_context *context, t_env *env, char *name,
		char *value)
{
	while (env)
	{
		if (ft_strncmp(env->var, name, ft_strlen(name)) == 0)
		{
			gc_unlock(env->value, context->shell->gcl);
			env->value = ft_strdup(value);
			return ;
		}
		env = env->next;
	}
	t_env (*new_env) = malloc(sizeof(t_env));
	if (!new_env)
	{
		gc_cleanup(context->shell->gcl);
		ft_dprintf(2, "Error: echec malloc new_env.\n");
		exit (ENOMEM);
	}
	new_env->var = ft_strdup(name);
	new_env->value = ft_strdup(value);
	free(value);
	if (!new_env->value || !new_env->var)
	{
		gc_cleanup(context->shell->gcl);
		ft_dprintf(2, "Error: echec malloc new_env->value.\n");
		exit (ENOMEM);
	}
	new_env->next = NULL;
	t_env (*current) = env;
	while (current && current->next != NULL)
		current = current->next;
	if (current)
		current->next = new_env;
}

static int	st_none_chdir(t_exec_context *context)
{
	char	*pwd;
	char	*cwd;
	t_env	*tmp_env;

	tmp_env = context->shell->env_data->env;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		pwd = st_get_env_value(tmp_env, "PWD");
		st_set_env_value(context, tmp_env, "OLDPWD", pwd);
		st_set_env_value(context, tmp_env, "PWD", cwd);
		free(cwd);
		return (0);
	}
	else
	{
		ft_dprintf(2, "minishell: chdir: error retrieving "
			"current directory: getcwd: cannot access parent "
			"directories: No such file or directory\n");
		return (1);
	}
}

static char	*st_cd_with_option(t_shell *shell, char *argv)
{
	char *(cd) = NULL;
	if (ft_strlen(argv) == 1)
	{
		if (!ft_strncmp(argv, "~", 2))
		{
			cd = st_get_env_value(shell->env_data->env, "HOME");
			if (!cd)
				return (ft_dprintf(2, "minishell: cd: HOME not set\n"), NULL);
		}
		else if (!ft_strncmp(argv, "-", 2))
		{
			cd = st_get_env_value(shell->env_data->env, "OLDPWD");
			if (!cd)
				return (ft_dprintf(2, "minishell: cd: OLDPWD not set\n"), NULL);
			ft_printf("%s\n", cd);
		}
	}
	if (!cd)
		cd = ft_strdup(argv);
	if (!cd)
	{
		gc_cleanup(shell->gcl);
		ft_dprintf(2, "Error: echec malloc cd\n");
		exit (ENOMEM);
	}
	return (cd);
}

int	ms_cd(t_cmd_node *cmd_node, t_exec_context *context)
{
	char *(cd) = NULL;
	if (cmd_node->argc == 1)
	{
		cd = st_get_env_value(context->shell->env_data->env, "HOME");
		if (!cd)
			return (ft_dprintf(2, "minishell: cd: HOME not set\n"), 1);
	}
	else if (cmd_node->argc == 3)
		return (ft_dprintf(2, "minishell: cd: too many arguments\n"), 1);
	else
	{
		cd = st_cd_with_option(context->shell, cmd_node->argv[1]);
		if (!cd)
			return (1);
	}
	if (chdir(cd))
		ft_dprintf(2, "minishell: cd: %s: No such file or directory\n",
			cmd_node->argv[1]);
	else
	{
		if (cmd_node->argc == 2)
			free(cd);
		return (st_none_chdir(context));
	}
	return (0);
}

/*int	ft_cd(t_infos *infos, t_tok *tmp)
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
