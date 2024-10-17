/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_register.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 10:25:20 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/02 15:26:44 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libgc.h"

void	gc_register(void *ptr, t_gc *gcl)
{
	t_gc_node	*new_node;

	if (!ptr)
		return ;
	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
	{
		gc_cleanup(gcl);
		write(2, "Error: GC node malloc failed.\n", 31);
		exit(EXIT_FAILURE);
	}
	new_node->ptr = ptr;
	new_node->is_marked = false;
	new_node->is_locked = false;
	new_node->is_array = false;
	new_node->fd = -1;
	new_node->temp_file = NULL;
	new_node->next = gcl->head;
	gcl->head = new_node;
}

static int	ft_strlen(const char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

static void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*mem_dest;
	unsigned char	*mem_src;

	if (!dest && !src)
		return (NULL);
	mem_dest = (unsigned char *)dest;
	mem_src = (unsigned char *)src;
	while (n-- && mem_dest && mem_src)
		*mem_dest++ = *mem_src++;
	return (dest);
}

static char	*ft_strdup(const char *s)
{
	char	*new_str;
	size_t	s_len;

	s_len = ft_strlen(s);
	new_str = (char *)malloc(sizeof(char) * (s_len + 1));
	if (!new_str)
	{
		errno = ENOMEM;
		return (NULL);
	}
	new_str = ft_memcpy(new_str, s, s_len + 1);
	return (new_str);
}

void	gc_temp_file_register(const char *filename, t_gc *gcl)
{
	t_gc_node	*new_node;

	if (!filename)
		return ;
	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
	{
		gc_cleanup(gcl);
		write(2, "Error: GC node malloc failed.\n", 31);
		exit(EXIT_FAILURE);
	}
	new_node->ptr = NULL;
	new_node->is_marked = false;
	new_node->is_locked = true;
	new_node->is_array = false;
	new_node->fd = -1;
	new_node->temp_file = ft_strdup(filename);
	if (!new_node->temp_file)
	{
		free(new_node);
		gc_cleanup(gcl);
		write(2, "Error: GC node malloc failed.\n", 31);
		exit(EXIT_FAILURE);
	}
	gc_register(new_node->temp_file, gcl);
	gc_lock(new_node->temp_file, gcl);
	new_node->next = gcl->head;
	gcl->head = new_node;
}
