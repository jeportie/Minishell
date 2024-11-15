/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wild_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:21:14 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/15 18:52:14 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

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

int	initialize_context(t_wildcard_context *ctx, const char *pattern,
		t_gc *gcl)
{
	ctx->pattern = pattern;
	ctx->capacity = 10;
	ctx->match_count = 0;
	ctx->gcl = gcl;
	ctx->matches = gc_malloc(sizeof(char *) * ctx->capacity, gcl);
	if (!ctx->matches)
		return (-1);
	gc_register(ctx->matches, gcl);
	return (0);
}

int	add_match(t_wildcard_context *ctx, const char *filename)
{
	char	*dup_name;
	char	**temp;

	if (ctx->match_count >= ctx->capacity)
	{
		ctx->capacity *= 2;
		temp = gc_realloc(ctx->matches, sizeof(char *) * ctx->capacity,
				ctx->gcl);
		if (!temp)
			return (-1);
		ctx->matches = temp;
	}
	dup_name = ft_strdup(filename);
	if (!dup_name)
	{
		printf("ft_strdup failed for filename: '%s'\n", filename);
		return (-1);
	}
	ctx->matches[ctx->match_count] = dup_name;
	gc_register(dup_name, ctx->gcl);
	printf("Added match: '%s'\n index: %d\n", ctx->matches[ctx->match_count], (int)ctx->match_count);
	ctx->match_count++;
	return (0);
}

int	check_match(t_wildcard_context *ctx, struct dirent *entry)
{
	if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		return (0);
	if (entry->d_name[0] == '.' && ctx->pattern[0] != '.')
		return (0);
	if (ms_pattern_match(ctx->pattern, entry->d_name))
	{
		if (add_match(ctx, entry->d_name) == -1)
			return (-1);
	}
	return (0);
}

char	**finalize_matches(t_wildcard_context *ctx)
{
	char	**final_matches;

	final_matches = gc_malloc(sizeof(char *) * (ctx->match_count + 1), ctx->gcl);
	if (!final_matches)
		return (NULL);
	final_matches[ctx->match_count] = NULL;
	return (final_matches);
}
