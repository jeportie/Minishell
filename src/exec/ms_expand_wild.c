/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wild.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 17:50:08 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/03 18:12:48 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include <dirent.h>

bool	ms_pattern_match(const char *pattern, const char *filename)
{
	if (*pattern == '\0' && *filename == '\0')
		return (true);
	if (*pattern == '*')
	{
		if (ms_pattern_match(pattern + 1, filename))
			return (true);
		if (*filename && ms_pattern_match(pattern, filename + 1))
			return (true);
		return (false);
	}
	if (*pattern == *filename)
		return (ms_pattern_match(pattern + 1, filename + 1));
	return (false);
}

char	**ms_expand_wild(const char *pattern, t_gc *gcl)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	size_t			match_count;
	size_t			capacity;

	dir = opendir(".");
	if (!dir)
	{
		perror("minishell: opendir failed");
		return (NULL);
	}
	capacity = 10;
	matches = gc_malloc(sizeof(char *) * capacity, gcl);
	if (!matches)
	{
		perror("minishell: memory allocation error");
		closedir(dir);
		return (NULL);
	}
	match_count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (ms_pattern_match(pattern, entry->d_name))
		{
			if (match_count >= capacity)
			{
				capacity *= 2;
				matches = gc_realloc(matches, sizeof(char *) * capacity, gcl);
				if (!matches)
				{
					perror("minishell: memory reallocation error");
					closedir(dir);
					return (NULL);
				}
			}
			matches[match_count] = ft_strdup(entry->d_name);
			gc_register(matches[match_count], gcl);
			match_count++;
		}
	}
	closedir(dir);
	matches = gc_realloc(matches, sizeof(char *) * (match_count + 1), gcl);
	matches[match_count] = NULL;
	return (matches);
}
