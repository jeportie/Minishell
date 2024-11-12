/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wild.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 17:50:08 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 16:38:05 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

char	**ms_expand_wild(const char *pattern, t_gc *gcl)
{
	DIR					*dir;
	struct dirent		*entry;
	t_wildcard_context	ctx;
	char				**final_matches;

	if (pattern == NULL)
		return (NULL);
	if (initialize_context(&ctx, pattern, gcl) == -1)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (check_match(&ctx, entry) == -1)
		{
			closedir(dir);
			return (NULL);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	final_matches = finalize_matches(&ctx);
	return (final_matches);
}
