/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_proc_info.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:16:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 19:34:40 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

void	print_proc_info(const t_proc_manager *manager)
{
	const t_proc_info	*current = manager->head;

	printf("Process Manager - Active Processes:\n");
	printf("Title\t\tPID\tParent PID\tLevel\tFD_in\tFD_out\tFD_err\tHeredoc\n");
	printf("----------------------------------------------------------------");
	printf("----------------\n");
	while (current)
	{
		printf("%s\t\t%d\t%d\t\t%d\t%d\t%d\t%d\t%d\n",
			current->title,
			current->pid,
			current->parent_pid,
			current->child_lvl,
			current->fd_in,
			current->fd_out,
			current->fd_error,
			current->is_heredoc);
		current = current->next;
	}
	printf("----------------------------------------------------------------");
	printf("----------------\n");
	printf("\n");
}
