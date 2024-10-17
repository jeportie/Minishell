/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_nest_register.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 10:23:33 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/02 15:25:55 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libgc.h"

void	gc_nest_register(void *ptr, t_gc *gcl)
{
	t_gc_node	*new_node;
	void		**array;

	if (!ptr)
		return ;
	array = (void **)ptr;
	while (*array)
	{
		gc_register(*array, gcl);
		array++;
	}
	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
	{
		gc_cleanup(gcl);
		write(2, "Error: GC node malloc failed.\n", 31);
		exit(EXIT_FAILURE);
	}
	new_node->ptr = ptr;
	new_node->is_marked = false;
	new_node->is_locked = false;
	new_node->is_array = true;
	new_node->fd = -1;
	new_node->temp_file = NULL;
	new_node->next = gcl->head;
	gcl->head = new_node;
}
