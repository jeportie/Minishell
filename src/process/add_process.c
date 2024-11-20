/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:16:22 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 17:49:31 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

t_proc_info	*add_process(t_proc_manager *manager, t_proc_params *params)
{
	t_proc_info	*new_process;

	if (!manager || !params)
		return (NULL);
	new_process = gc_malloc(sizeof(t_proc_info), manager->gcl);
	if (!new_process)
		return (NULL);
	new_process->pid = params->pid;
	new_process->parent_pid = params->parent_pid;
	new_process->child_lvl = params->child_lvl;
	new_process->fd_in = params->fd_in;
	new_process->fd_out = params->fd_out;
	new_process->fd_error = params->fd_error;
	new_process->fd_original_stdout = -1;
	new_process->is_heredoc = params->is_heredoc;
	new_process->title = params->title;
	new_process->next = manager->head;
	manager->head = new_process;
	manager->process_count++;
	return (new_process);
}
