/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 16:48:33 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/03 17:24:32 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	ms_handle_error(const char *msg, t_gc *gcl)
{
	ft_dprintf(STDERR_FILENO, msg);
	if (gcl)
		gc_cleanup(gcl);
	return (1);
}
