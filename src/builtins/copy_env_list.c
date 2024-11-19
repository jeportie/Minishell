/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 12:55:31 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/19 13:01:27 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static char	*st_get_value(t_env *current, t_gc *gcl)
{
	char	*str;

	if (current->value)
	{
		str = gc_strdup(current->value);
		if (!str)
			echec_malloc(gcl, "new_node->value");
		return (str);
	}
	return (NULL);
}

t_env	*copy_env_list(t_env *env, t_gc *gcl)
{
	t_env *(new_node) = NULL;
	t_env *(new_list) = NULL;
	t_env *(current) = env;
	t_env *(last) = NULL;
	while (current)
	{
		new_node = gc_malloc(sizeof(t_env), gcl);
		if (!new_node)
			echec_malloc(gcl, "new_node");
		new_node->var = gc_strdup(current->var);
		if (!new_node->var)
			echec_malloc(gcl, "new_node->var");
		if (current->value)
			new_node->value = st_get_value(current, gcl);
		new_node->next = NULL;
		if (!new_list)
			new_list = new_node;
		else
			last->next = new_node;
		last = new_node;
		current = current->next;
	}
	return (new_list);
}
