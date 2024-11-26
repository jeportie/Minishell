/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   back_to_parent_stdout.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:07:40 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/26 16:25:28 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

int	back_to_parent_stdout(t_proc_manager *manager, pid_t pid)
{
	t_proc_info	*current;

	current = manager->head;
	while (current)
	{
		if (current->pid == pid)
		{
			if (current->fd_original_stdout >= 0)
			{
				if (dup2(current->fd_original_stdout, STDOUT_FILENO) == -1)
				{
					ft_dprintf(STDERR_FILENO,
						"Failed to restore parents STDOUT");
					return (-1);
				}
				return (0);
			}
			break ;
		}
		current = current->next;
	}
	return (-1);
}
