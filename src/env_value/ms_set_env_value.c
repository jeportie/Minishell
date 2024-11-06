/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_set_env_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:49:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 21:56:33 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static void	st_add_new_env_value(t_shell *shell, char *name, char *value)
{
	t_env	*node;
	char	*var;

	var = ft_strjoin(name, "=");
	if (!var)
		echec_malloc(shell->gcl, "var");
	var = ft_strjoin_free(var, value);
	if (!var)
		echec_malloc(shell->gcl, "var");
	node = ms_env_create_node(shell, var);
	ms_env_add_back(&shell->env_data->env, node);
}

void	ms_set_env_value(t_shell *shell, char *name, char *value)
{
	t_env	*tmp_env;

	tmp_env = shell->env_data->env;
	while (tmp_env)
	{
		if (ft_strncmp(tmp_env->var, name, ft_strlen(name) + 1) == 0)
		{
			gc_unlock(tmp_env->value, shell->gcl);
			tmp_env->value = ft_strdup(value);
			if (!tmp_env->value)
				echec_malloc(shell->gcl, "new_node");
			gc_register(tmp_env->value, shell->gcl);
			gc_lock(tmp_env->value, shell->gcl);
			return ;
		}
		tmp_env = tmp_env->next;
	}
	st_add_new_env_value(shell, name, value);
	gc_collect(shell->gcl);
}
