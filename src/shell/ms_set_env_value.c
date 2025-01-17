/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_set_env_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:49:42 by gmarquis          #+#    #+#             */
/*   Updated: 2024/11/22 15:11:25 by gmarquis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

char	*gc_strjoin(char *s1, char *s2, t_gc *gcl)
{
	char	*joined_str;
	size_t	len1;
	size_t	len2;
	size_t	total_len;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	total_len = len1 + len2;
	joined_str = (char *)gc_malloc(sizeof(char) * (total_len + 1), gcl);
	if (!joined_str)
		return (NULL);
	ft_memcpy(joined_str, s1, len1);
	ft_memcpy(joined_str + len1, s2, len2);
	joined_str[total_len] = '\0';
	return (joined_str);
}

static void	st_add_new_env_value(t_shell *shell, char *name, char *value)
{
	t_env	*node;
	char	*var;

	var = gc_strjoin(name, "=", shell->gcl);
	if (!var)
		echec_malloc(shell->gcl, "var");
	var = gc_strjoin(var, value, shell->gcl);
	if (!var)
		echec_malloc(shell->gcl, "var");
	node = ms_env_create_node(shell, var);
	ms_env_add_back(&shell->env_data->env, node);
}

void	ms_set_env_value(t_shell *shell, char *name, char *value)
{
	t_env *(tmp_env) = shell->env_data->env;
	while (tmp_env)
	{
		if (!ft_strncmp(tmp_env->var, name, ft_strlen(name) + 1))
		{
			tmp_env->value = gc_strdup(value, shell->gcl);
			if (!tmp_env->value)
				echec_malloc(shell->gcl, "tmp_env->value");
			gc_lock(tmp_env->value, shell->gcl);
			return ;
		}
		tmp_env = tmp_env->next;
	}
	st_add_new_env_value(shell, name, value);
}
