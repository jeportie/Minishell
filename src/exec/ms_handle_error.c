/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 16:48:33 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/17 15:56:29 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	ms_handle_error(const char *msg, int exit_status, t_gc *gcl)
{
	ft_dprintf(STDERR_FILENO, msg);
	gcl = gcl;
//	if (gcl)
//		gc_cleanup(gcl);
	return (exit_status);
}
