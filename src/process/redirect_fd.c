/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:59:43 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 12:04:23 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

int	redirect_fd(int old_fd, int new_fd)
{
	if (dup2(new_fd, old_fd) == -1)
	{
		ft_dprintf(STDERR_FILENO, "dup2 failed to redirect file director.\n");
		return (-1);
	}
	return (0);
}

int	restore_fd(int fd, int saved_fd)
{
	if (dup2(saved_fd, fd) == -1)
	{
		ft_dprintf(STDERR_FILENO, "dup2 failed to restore file director.\n");
		return (-1);
	}
	return (0);
}
