/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:36:14 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/04 09:10:42 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**st_create_envp(t_shell *shell, t_env_data *env_data)
{
	char	*cwd;
	char	**envp;
	char	*tmp;

	envp = (char **)gc_malloc(sizeof(char *) * 3, shell->gcl);
	cwd = getcwd(NULL, 0);
	env_data->path = "";
	env_data->term = "";
	env_data->shell = "";
	envp[0] = "SHLVL=0";
	envp[1] = "PWD=";
	tmp = gc_strjoin(envp[1], cwd, shell->gcl);
	envp[1] = tmp;
	free(cwd);
	envp[2] = NULL;
	return (envp);
}

static void	st_shell_lvl_helper(int i, t_env *current, t_gc *gcl)
{
	if (i >= INT_MAX)
		current->value = "0";
	else
	{
		current->value = ft_itoa(i + 1);
		gc_register(current->value, gcl);
		gc_lock(current->value, gcl);
	}
	return ;
}

void	ms_increment_shell_lvl(t_env *env, t_gc *gcl)
{
	t_env	*current;
	long	i;

	current = env;
	while (current)
	{
		if (!ft_strncmp(current->var, "SHLVL", 6))
		{
			i = ft_atol(current->value);
			if (i >= 999 && i <= INT_MAX)
			{
				ft_dprintf(2, "minishell: warning: shell level (%d)"
					" too high, resetting to 1\n", i);
				current->value = "1";
				return ;
			}
			else
				st_shell_lvl_helper(i, current, gcl);
			i++;
		}
		current = current->next;
	}
}

t_env_data	*ms_init_env(char **envp, t_shell *shell)
{
	t_env_data	*env_data;
	t_env		*node;

	int (i) = -1;
	if (!shell)
		exit(EXIT_FAILURE);
	env_data = (t_env_data *)gc_malloc(sizeof(t_env_data), shell->gcl);
	gc_lock(env_data, shell->gcl);
	ft_memset(env_data, 0, sizeof(t_env_data));
	if (envp[0])
		env_data->env_exist = true;
	else
		envp = st_create_envp(shell, env_data);
	env_data->env = NULL;
	while (envp[++i])
	{
		node = ms_env_create_node(shell, envp[i]);
		if (node->var)
			ms_env_add_back(&env_data->env, node);
	}
	ms_increment_shell_lvl(env_data->env, shell->gcl);
	init_fix_value(env_data);
	gc_collect(shell->gcl);
	return (env_data);
}
