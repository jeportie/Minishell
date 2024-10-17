/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_collect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 18:23:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/02 15:22:41 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libgc.h"
#include <stdarg.h>

static void	gc_delete_node(t_gc_node **prev, t_gc_node **current, t_gc *gcl)
{
	t_gc_node	*next_node;
	t_gc		*tmp;

	tmp = gcl;
	next_node = (*current)->next;
	if (*prev)
		(*prev)->next = next_node;
	else
		tmp->head = next_node;
	if ((*current)->ptr)
		free((*current)->ptr);
	free(*current);
	*current = next_node;
	*gcl = *tmp;
}

void	gc_collect(t_gc *gcl)
{
	t_gc_node	*current;
	t_gc_node	*prev;

	current = gcl->head;
	prev = NULL;
	while (current)
	{
		if (!current->is_marked && !current->is_locked)
			gc_delete_node(&prev, &current, gcl);
		else
		{
			current->is_marked = false;
			prev = current;
			current = current->next;
		}
	}
}
