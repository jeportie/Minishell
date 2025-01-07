/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_env_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:48:43 by gmarquis          #+#    #+#             */
/*   Updated: 2024/11/15 11:23:37 by gmarquis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

char	*ms_get_env_value(t_env *env, char *name, int error_code)
{
	t_env	*tmp;
	int		var_verif;

	if (!name)
		return (NULL);
	if (!ft_strncmp(name, "?", 2))
		return (ft_itoa(error_code));
	var_verif = valide_var(name);
	if (var_verif == 0)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->var, name, var_verif + 1))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
