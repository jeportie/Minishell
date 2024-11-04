/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:25:19 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 13:33:00 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

/*void	ft_unset_utils(t_infos *infos, t_env *prev, t_env *current)
{
	if (prev)
		prev->next = current->next;
	else
		infos->tmp_env = current->next;
	current->name_folder = ft_free_str(current->name_folder);
	current->value_folder = ft_free_str(current->value_folder);
	free(current);
	current = NULL;
}

int	ms_unset(t_infos *infos, char **cmd)
{
	t_env *(current) = NULL;
	t_env *(prev) = NULL;
	int (i) = 1;
	if (!cmd[i])
		return (0);
	while (cmd[i])
	{
		current = infos->tmp_env;
		while (current)
		{
			if (!ft_strncmp(current->name_folder, cmd[i],
					ft_strlen(cmd[i]) + 1))
			{
				ft_unset_utils(infos, prev, current);
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
	return (0);
}*/
