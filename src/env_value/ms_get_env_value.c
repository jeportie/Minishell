/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_env_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:48:43 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 20:24:29 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env_value.h"

char	*ms_get_env_value(t_env *env, char *name)
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
