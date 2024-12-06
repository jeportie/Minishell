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

static int	st_get_lasts_slash(const char *pattern)
{
	int	i;

	i = ft_strlen(pattern);
	while (pattern[i] != '/')
		i--;
	return (i);
}

static char	*st_get_path_dir(const char *pattern, t_gc *gcl)
{
	char	*path;
	int		len;

	if (ft_strchr(pattern, '/'))
	{
		len = st_get_lasts_slash(pattern) + 1;
		path = gc_malloc(len * sizeof(char), gcl);
		ft_strlcpy(path, pattern, len+1);
		path[len] = '\0';
	}
	else
	{
		path = gc_malloc(2 * sizeof (char), gcl);
		path[0] = '.';
		path[1] = '\0';
	}
	return (path);
}

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
	dir = opendir(st_get_path_dir(pattern, gcl));
	if (!dir)
		return (NULL);
	//printf("ici\n");
	entry = readdir(dir);
	while (entry != NULL)
	{
		//printf("-> %s\n", entry->d_name);
		if (check_match(ctx, entry) == -1)
		{
			closedir(dir);
			return (NULL);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	//printf("la\n");
	return (ctx);
}
