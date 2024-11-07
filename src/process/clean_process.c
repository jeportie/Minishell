/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:56:11 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 14:10:38 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

void	cleanup_proc(t_proc_manager *manager, pid_t pid)
{
	t_proc_info	*current;

	current = manager->head;
	t_proc_info *(prev) = NULL;
	while (current)
	{
		if (current->pid == pid)
		{
			if (current->fd_in >= 0)
				safe_close(current->fd_in);
			if (current->fd_out >= 0)
				safe_close(current->fd_out);
			if (current->fd_error >= 0)
				safe_close(current->fd_error);
			if (prev)
				prev->next = current->next;
			else
				manager->head = current->next;
			gc_free(current, manager->gcl);
			manager->process_count--;
			return ;
		}
		prev = current;
		current = current->next;
	}
}
