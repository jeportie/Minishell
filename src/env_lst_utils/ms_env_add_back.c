/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_add_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:56:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/01 15:01:56 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_env_add_back(t_env **env, t_env *new_node)
{
	t_env	*current;

	if (!env || !new_node)
		return ;
	if (!*env)
	{
		*env = new_node;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
