/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_fd_register.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 10:18:49 by jeportie          #+#    #+#             */
/*   Updated: 2024/09/30 13:53:17 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libgc.h"

void	gc_fd_register(int fd, t_gc gcl)
{
	t_gc_node	*new_node;

	if (!fd)
		return ;
	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
	{
		return ;
	}
	new_node->ptr = NULL;
	new_node->is_marked = false;
	new_node->is_locked = true;
	new_node->is_array = false;
	new_node->fd = fd;
	new_node->temp_file = NULL;
	new_node->next = gcl.head;
	gcl.head = new_node;
}
