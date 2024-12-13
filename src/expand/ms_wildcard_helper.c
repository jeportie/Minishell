/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_wildcard_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 20:43:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/12 11:12:09 by gmarquis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

void	sort_wild(t_wildcard_context *ctx)
{
	int		i;
	char	*swap;

	i = 1;
	if (!ctx->matches)
		return ;
	while (ctx->match_count > 1 && ctx->matches[i])
	{
		if (strcmp(ctx->matches[i - 1], ctx->matches[i]) > 0)
		{
			swap = ctx->matches[i];
			ctx->matches[i] = ctx->matches[i - 1];
			ctx->matches[i - 1] = swap;
			i = 1;
		}
		else
			i++;
	}
}

int	add_matches(t_wildcard_context *ctx, const char *match)
{
	char	**new_matches;

	new_matches = gc_realloc(ctx->matches, (ctx->match_count + 2)
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
	ctx->matches[ctx->match_count] = NULL;
	return (1);
}
