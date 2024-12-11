/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_wildcard_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:43:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/11 13:05:56 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

int	st_add_match(t_wildcard_context *ctx, const char *match)
{
	char	**new_matches;

	new_matches = gc_realloc(ctx->matches, (ctx->match_count + 1)
			* sizeof(char *), ctx->gcl);
	if (!new_matches)
		return (0);
	gc_lock(new_matches, ctx->gcl);
	ctx->matches = new_matches;
	ctx->matches[ctx->match_count] = gc_strdup(match, ctx->gcl);
	gc_lock(ctx->matches[ctx->match_count], ctx->gcl);
	if (!ctx->matches[ctx->match_count])
		return (0);
	ctx->match_count++;
	return (1);
}
