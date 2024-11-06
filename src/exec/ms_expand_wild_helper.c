/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wild_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:21:14 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 15:27:07 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/* 
 * Function: ms_pattern_match
 * --------------------------
 * Recursively matches a pattern with a filename.
 *
 * pattern: The wildcard pattern to match.
 * filename: The filename to compare against the pattern.
 *
 * returns: true if the filename matches the pattern, false otherwise.
 */
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

/* 
 * Function: initialize_context
 * ----------------------------
 * Initializes the wildcard context structure.
 *
 * ctx: Pointer to the wildcard context to initialize.
 * pattern: The wildcard pattern to use for matching.
 * gcl: Garbage collector instance for memory management.
 *
 * returns: 0 on success, -1 on failure.
 */

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

/* 
 * Function: add_match
 * -------------------
 * Adds a matched filename to the context's matches array.
 *
 * ctx: Pointer to the wildcard context.
 * filename: The filename to add to the matches.
 *
 * returns: 0 on success, -1 on failure.
 */

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
		return (-1);
	ctx->matches[ctx->match_count] = dup_name;
	gc_register(dup_name, ctx->gcl);
	ctx->match_count++;
	return (0);
}

/* 
 * Function: check_match
 * ---------------------
 * Checks if a directory entry matches the pattern and adds it to matches 
 * if it does.
 *
 * ctx: Pointer to the wildcard context.
 * entry: The directory entry to check.
 *
 * returns: 0 on success, -1 on failure.
 */

int	check_match(t_wildcard_context *ctx, struct dirent *entry)
{
	if (entry->d_name[0] == '.' && ctx->pattern[0] != '.')
		return (0);
	if (ms_pattern_match(ctx->pattern, entry->d_name))
	{
		if (add_match(ctx, entry->d_name) == -1)
			return (-1);
	}
	return (0);
}

/* 
 * Function: finalize_matches
 * --------------------------
 * Finalizes the matches array by reallocating it to fit the exact number of
 * matches
 * and adding a NULL terminator.
 *
 * ctx: Pointer to the wildcard context.
 *
 * returns: Pointer to the finalized matches array, or NULL on failure.
 */

char	**finalize_matches(t_wildcard_context *ctx)
{
	char	**final_matches;

	final_matches = gc_realloc(ctx->matches, sizeof(char *)
			* (ctx->match_count + 1), ctx->gcl);
	if (!final_matches)
		return (NULL);
	final_matches[ctx->match_count] = NULL;
	return (final_matches);
}
