/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wild.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 17:50:08 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/20 13:14:52 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include <unistd.h>

t_wildcard_context	*ms_expand_wild(const char *pattern, t_gc *gcl)
{
	DIR					*dir;
	struct dirent		*entry;
	t_wildcard_context	*ctx;

	if (!pattern)
		return (NULL);
	ctx = gc_malloc(sizeof(t_wildcard_context), gcl);
	if (initialize_context(ctx, pattern, gcl) == -1)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (check_match(ctx, entry) == -1)
		{
			closedir(dir);
			return (NULL);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (ctx);
}
