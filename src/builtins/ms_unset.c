/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:25:19 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/04 13:49:54 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static void	st_unset_utils(t_exec_context *context, t_env *prev, t_env *current)
{
	if (prev)
		prev->next = current->next;
	else
		context->shell->env_data->env = current->next;
	current = NULL;
}

int	ms_unset(t_cmd_node *cmd_node, t_exec_context *context)
{
	t_env *(current) = NULL;
	t_env *(prev) = NULL;
	int (i) = 1;
	if (cmd_node->argc == 1)
		return (0);
	while (cmd_node->argv[i])
	{
		current = context->shell->env_data->env;
		while (current)
		{
			if (!ft_strncmp(current->var, cmd_node->argv[i],
					ft_strlen(cmd_node->argv[i]) + 1))
			{
				st_unset_utils(context, prev, current);
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
	return (0);
}
