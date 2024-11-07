/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:07:27 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 11:14:17 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

void	clean_manager(t_proc_manager *manager)
{
	t_proc_info	*current;
	t_proc_info	*next;

	if (!manager)
		return ;
	current = manager->head;
	while (current)
	{
		next = current->next;
		if (current->fd_in >= 0)
			safe_close(current->fd_in);
		if (current->fd_out >= 0)
			safe_close(current->fd_out);
		if (current->fd_error >= 0)
			safe_close(current->fd_error);
		gc_free(current, manager->gcl);
		current = next;
	}
	manager->head = NULL;
	manager->process_count = 0;
}
