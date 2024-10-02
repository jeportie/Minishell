/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:36:14 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/02 15:28:25 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	st_create_envp(char **envp, t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	shell->env_data.path
		= "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	envp[0] = "SHLVL=1";
	envp[1] = "PWD=";
	ft_strlcat(envp[1], cwd, ft_strlen(envp[1]));
	gc_register(envp[1], shell->gcl);
	gc_lock(envp[1], shell->gcl);
	free(cwd);
	envp[2] = NULL;
}

t_env_data	ms_init_env(char **envp, t_shell *shell)
{
	t_env_data	env_data;
	t_env		*node;
	int			i;

	ft_memset(&env_data, 0, sizeof(t_env_data));
	if (envp)
		env_data.env_exist = true;
	else
		st_create_envp(envp, shell);
	env_data.env = gc_malloc(sizeof(t_env), shell->gcl);
	gc_lock(env_data.env, shell->gcl);
	ft_memset(env_data.env, 0, sizeof(t_env_data));
	i = 0;
	while (envp[i])
	{
		node = ms_env_create_node(shell, envp[i]);
		ms_env_add_back(&env_data.env, node);
		i++;
	}
	return (env_data);
}
