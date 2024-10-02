/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_create_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:15:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/02 15:28:49 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*ms_env_create_node(t_shell *shell, const char *env_line)
{
	char	**splits;
	t_env	*new_node;

	if (!env_line)
		return (NULL);
	splits = ft_split(env_line, '=');
	gc_nest_register(splits, shell->gcl);
	gc_lock(splits, shell->gcl);
	new_node = gc_malloc(sizeof(t_env), shell->gcl);
	gc_lock(new_node, shell->gcl);
	ft_memset(new_node, 0, sizeof(t_env));
	new_node->var = splits[0];
	new_node->value = splits[1];
	return (new_node);
}
