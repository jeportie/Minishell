/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_proc_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:55:09 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/26 16:27:47 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

t_proc_manager	*init_manager(t_gc *gcl)
{
	t_proc_manager	*manager;

	manager = gc_malloc(sizeof(t_proc_manager), gcl);
	if (!manager)
		return (NULL);
	gc_lock(manager, gcl);
	ft_memset(manager, 0, sizeof(t_proc_manager));
	manager->gcl = gcl;
	return (manager);
}
