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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

static int  st_add_match(t_wildcard_context *ctx, const char *match)
{
    char **new_matches;
    
    new_matches = gc_realloc(ctx->matches, (ctx->match_count + 1)
        * sizeof(char *), ctx->gcl);
    if (!new_matches)
        return (0);
    ctx->matches = new_matches;
    ctx->matches[ctx->match_count] = gc_strdup(match, ctx->gcl);
    gc_lock(ctx->matches[ctx->match_count], ctx->gcl);
    if (!ctx->matches[ctx->match_count])
        return (0);
    ctx->match_count++;
    return (1);
}

static int  st_matches_pattern(const char *filename, const char *pattern)
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

static void st_process_directory(DIR *dir, const char *base_path, const char *current_pattern,
    const char *remaining_pattern, char *slash, t_wildcard_context *ctx)
{
    struct dirent *entry;
    struct stat st;
    char full_path[512];

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue ;
        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);
        if (stat(full_path, &st) == -1)
            continue ;
        if (slash && S_ISDIR(st.st_mode) && st_matches_pattern(entry->d_name, current_pattern))
            expand_glob_recursive(full_path, remaining_pattern, ctx);
        else if (!slash && st_matches_pattern(entry->d_name, current_pattern))
        {
            if (!st_add_match(ctx, full_path))
            {
                ft_dprintf(2, "Echec: ajout match\n");
                break ;
            }
        }
    }
}

void expand_glob_recursive(const char *base_path, const char *pattern, t_wildcard_context *ctx)
{
    char    *slash;
    DIR     *dir;
    char current_pattern[256] = {0};
    char remaining_pattern[256] = {0};

    slash = ft_strchr(pattern, '/');
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
    st_process_directory(dir, base_path, current_pattern, remaining_pattern, slash, ctx);
    closedir(dir);
}


t_wildcard_context *ms_expand_wild(const char *pattern, t_gc *gcl)
{
    t_wildcard_context  *ctx;
    
    ctx = gc_malloc(sizeof(t_wildcard_context), gcl);
    if (!ctx)
        return (NULL);
    ctx->pattern = pattern;
    ctx->matches = NULL;
    ctx->match_count = 0;
    ctx->gcl = gcl;
    expand_glob_recursive(".", pattern, ctx);
    return (ctx);
}
