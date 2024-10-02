/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:15:11 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/02 15:30:09 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libgc.h"

static void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*mem_src;

	mem_src = (unsigned char *)s;
	while (n--)
		*mem_src++ = (unsigned char)c;
	return (s);
}

void	*gc_init(void)
{
	t_gc	*gcl;

	gcl = malloc(sizeof(t_gc));
	if (!gcl)
	{
		write(2, "Error: GC allocation failed.\n", 29);
		exit(EXIT_FAILURE);
	}
	ft_memset(gcl, 0, sizeof(t_gc));
	return (gcl);
}
