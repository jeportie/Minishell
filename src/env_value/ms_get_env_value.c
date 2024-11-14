/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_env_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:48:43 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/14 18:12:34 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

char	*ms_get_env_value(t_env *env, char *name)
{
	t_env	*tmp;
	int		var_verif;

	if (!name)
		return (NULL);
	var_verif = valide_var(name);
	if (var_verif == 0)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->var, name, var_verif + 1) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
