/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:53:49 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/10 16:07:08 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

int	safe_open(const char *filepath, int flags, int mode)
{
	int		fd;

	fd = open(filepath, flags, mode);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "Open failed\n");
		return (-1);
	}
	return (fd);
}
