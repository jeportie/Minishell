/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_proc_info.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:16:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/26 16:37:00 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"
#include "../../include/minishell.h"

void	print_proc_info(const t_proc_manager *manager)
{
	const t_proc_info	*current = manager->head;
	int					fd;

	if (DEBUG == 0)
		return ;
	fd = open(PRINT_INFOS, O_WRONLY | O_APPEND , COPY_MODE);
	while (current)
	{
		ft_dprintf(fd, "%s\t\t%d\t%d\t\t%d\t%d\t%d\t%d\t%d\n",
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
	ft_dprintf(fd, "----------------------------------------------------------------");
	ft_dprintf(fd, "----------------\n");
	ft_dprintf(fd, "\n");
	close(fd);
}
