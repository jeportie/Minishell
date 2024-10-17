/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 10:09:32 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/02 15:21:35 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libgc.h"

void	gc_cleanup(t_gc *gcl)
{
	t_gc_node	*current;
	t_gc_node	*next;

	current = gcl->head;
	while (current)
	{
		next = current->next;
		if (current->temp_file)
			unlink(current->temp_file);
		if (current->fd > -1)
			close(current->fd);
		else if (current->ptr)
			free(current->ptr);
		free(current);
		current = next;
	}
	gcl->head = NULL;
}
