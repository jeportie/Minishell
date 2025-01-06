/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:30:00 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/06 15:34:45 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ms_getenv(const char *name, t_env_data *env_data)
{
	t_env	*current;
	size_t	name_len;

	if (!name || !env_data || !env_data->env)
		return (NULL);
	current = env_data->env;
	name_len = ft_strlen(name);
	while (current)
	{
		if (ft_strncmp(current->var, name, name_len) == 0
			&& current->var[name_len] == '\0')
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
