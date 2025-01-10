/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echec_malloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:58:08 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 10:40:54 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

void	ms_echec_malloc(t_gc *gcl, char *message)
{
	gc_cleanup(gcl);
	ft_dprintf(2, "Error: echec malloc %s\n", message);
	exit (ENOMEM);
}
