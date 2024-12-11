/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wild.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 17:50:08 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/11 13:05:10 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

static int	st_matches_p(const char *filename, const char *pattern)
{
	const char *(ptr_pat) = pattern;
	const char *(ptn_file) = filename;
	const char *(last_wild) = NULL;
	const char *(wild_match) = NULL;
	while (*ptn_file)
	{
		if (*ptr_pat == '*')
		{
			last_wild = ptr_pat++;
			wild_match = ptn_file;
		}
		else if (*ptr_pat++ == *ptn_file++)
			;
		else if (last_wild)
		{
			ptr_pat = last_wild + 1;
			ptn_file = ++wild_match;
		}
		else
			return (0);
	}
	while (*ptr_pat == '*')
		ptr_pat++;
	return (*ptr_pat == '\0');
}

static int	st_process_entry(struct dirent *entry, t_directory_context *dctx)
{
	struct stat	st;
	char		full_path[512];

	snprintf(full_path, sizeof(full_path), "%s/%s",
		dctx->base_path, entry->d_name);
	if (stat(full_path, &st) == -1)
		return (1);
	if (dctx->slash && S_ISDIR(st.st_mode)
		&& st_matches_p(entry->d_name, dctx->c_p))
		expand_glob_recursive(full_path, dctx->r_p, dctx->ctx);
	else if (!dctx->slash && st_matches_p(entry->d_name, dctx->c_p))
	{
		if (!st_add_match(dctx->ctx, full_path))
		{
			ft_dprintf(2, "Echec: ajout match\n");
			return (0);
		}
	}
	return (1);
}

static void	st_process_directory(DIR *dir, t_directory_context *dctx)
{
	struct dirent	*entry;

	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (!st_process_entry(entry, dctx))
			break ;
		entry = readdir(dir);
	}
}

void	expand_glob_recursive(const char *base_path, const char *pattern,
		t_wildcard_context *ctx)
{
	DIR	*dir;

	char (current_pattern)[256] = {0};
	char (remaining_pattern)[256] = {0};
	char *(slash) = ft_strchr(pattern, '/');
	if (slash)
	{
		ft_strlcpy(current_pattern, pattern, (slash - pattern) + 1);
		ft_strlcpy(remaining_pattern, slash + 1, sizeof(remaining_pattern));
	}
	else
		ft_strlcpy(current_pattern, pattern, sizeof(current_pattern));
	dir = opendir(base_path);
	if (!dir)
	{
		ft_dprintf(2, "Echec: opendir\n");
		return ;
	}
	t_directory_context (dctx) = {slash, base_path, current_pattern,
	remaining_pattern, ctx};
	st_process_directory(dir, &dctx);
	closedir(dir);
}

t_wildcard_context	*ms_expand_wild(const char *pattern, t_gc *gcl)
{
	t_wildcard_context	*ctx;

	ctx = gc_malloc(sizeof(t_wildcard_context), gcl);
	gc_lock(ctx, gcl);
	if (!ctx)
		return (NULL);
	memset(ctx, 0, sizeof(t_wildcard_context));
	ctx->pattern = pattern;
	ctx->gcl = gcl;
	expand_glob_recursive(".", pattern, ctx);
	return (ctx);
}
