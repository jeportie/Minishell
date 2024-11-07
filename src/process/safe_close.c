/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_close.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:56:53 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 11:58:26 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

int	safe_close(int fd)
{
	if (close(fd) == -1)
	{
		ft_dprintf(STDERR_FILENO, "Close failed\n");
		return (-1);
	}
	return (fd);
}
