/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:36:14 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/01 16:20:52 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env_data ms_init_env(char **envp, t_shell *shell)
{
	t_env_data	env_data;
	t_env		*node;

	env_data.env_exist = true;
	env_data.env = NULL;
	env_data.path = NULL;
	if (envp)
		env_data.env_exist = true;
	else
	{
		env_data.env = gc_malloc(sizeof(t_env), *shell->gcl);
		if (!env_data.env)
		{
			gc_cleanup(*shell->gcl);
		}
		node = ms_env_create_node(shell);
		ms_env_add_back(&env_data.env, node);
	}
	return (env_data);
}
