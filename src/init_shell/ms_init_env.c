/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:36:14 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/16 14:34:54 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*st_get_path(t_env_data *env_data)
{
	t_env	*current;
	int		trigger;

	trigger = 0;
	current = env_data->env;
	while (current)
	{
		if (current->var && !ft_strncmp(current->var, "PATH", 5))
		{
			trigger = 1;
			return (current->value);
			break ;
		}
		current = current->next;
	}
	if (trigger == 0)
		return ("/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	return (NULL);
}

static char	*st_get_term(t_env_data *env_data)
{
	t_env	*current;
	int		trigger;

	trigger = 0;
	current = env_data->env;
	while (current)
	{
		if (current->var && !ft_strncmp(current->var, "TERM", 5))
		{
			trigger = 1;
			return (current->value);
			break ;
		}
		current = current->next;
	}
	if (trigger == 0)
		return ("dumb");
	return (NULL);
}

static char	*st_get_shell(t_env_data *env_data)
{
	t_env	*current;
	int		trigger;

	trigger = 0;
	current = env_data->env;
	while (current)
	{
		if (current->var && !ft_strncmp(current->var, "SHELL", 5))
		{
			trigger = 1;
			return (current->value);
			break ;
		}
		current = current->next;
	}
	if (trigger == 0)
		return ("/bin/zsh");
	return (NULL);
}

static char	**st_create_envp(t_shell *shell, t_env_data *env_data)
{
	char	*cwd;
	char 	**envp;

	envp = (char **)gc_malloc(sizeof(char *) * 3, shell->gcl);
	//gc_lock(envp, shell->gcl);
	cwd = getcwd(NULL, 0);
	env_data->path = "";
	env_data->term = "";
	env_data->shell = "";
	envp[0] = "SHLVL=1";
	envp[1] = "PWD=";
	ft_strlcat(envp[1], cwd, ft_strlen(envp[1]));
	free(cwd);
	envp[2] = NULL;
	return (envp);
}

t_env_data	*ms_init_env(char **envp, t_shell *shell)
{
	t_env_data	*env_data;
	t_env		*node;
	int			i;

	if (!shell)
	{
		perror("Minishell: Error: Segfault\n");
		exit(EXIT_FAILURE);
	}
	env_data = (t_env_data *)gc_malloc(sizeof(t_env_data), shell->gcl);
	ft_memset(env_data, 0, sizeof(t_env_data));
	if (envp)
		env_data->env_exist = true;
	else
		envp = st_create_envp(shell, env_data);
	env_data->env = NULL;
	i = 0;
	while (envp[i])
	{
		node = ms_env_create_node(shell, envp[i]);
		if (node->var)
			ms_env_add_back(&env_data->env, node);
		i++;
	}
	env_data->path = st_get_path(env_data);
	env_data->term = st_get_term(env_data);
	env_data->shell = st_get_shell(env_data);
	return (env_data);
}
