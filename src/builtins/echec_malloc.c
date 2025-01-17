/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echec_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:58:08 by gmarquis          #+#    #+#             */
/*   Updated: 2024/11/12 15:40:06 by gmarquis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

void	echec_malloc(t_gc *gcl, char *message)
{
	gc_cleanup(gcl);
	ft_dprintf(2, "Error: echec malloc %s\n", message);
	exit (ENOMEM);
}
