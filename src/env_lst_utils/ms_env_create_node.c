/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_create_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:15:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/15 07:41:22 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*ms_env_create_node(t_shell *shell, const char *env_line)
{
	char	**splits;
	t_env	*new_node;

	if (!env_line || !shell)
		exit(EXIT_FAILURE);
	new_node = gc_malloc(sizeof(t_env), shell->gcl);
	gc_lock(new_node, shell->gcl);
	ft_memset(new_node, 0, sizeof(t_env));
	if (!ft_strchr(env_line, '='))
		new_node->var = (char *)env_line;
	else
	{
		splits = ft_split(env_line, '=');
		if (!splits)
			exit(EXIT_FAILURE);
		gc_nest_register(splits, shell->gcl);
		gc_lock(splits[0], shell->gcl);
		new_node->var = splits[0];
		if (!splits[1])
			splits[1] = "";
		gc_lock(splits[1], shell->gcl);
		new_node->value = splits[1];
	}
	return (new_node);
}
