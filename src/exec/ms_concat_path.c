/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_concat_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:57:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/03 18:34:01 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static void	ms_check_alloc(void *ptr, t_gc *gcl)
{
	if (!ptr)
	{
		ft_dprintf(STDERR_FILENO, "error: minishell: memory allocation error");
		gc_cleanup(gcl);
		exit(EXIT_FAILURE);
	}
}

char	*ms_concat_path(const char *path, const char *command, t_gc *gcl)
{
	char	*cmd_path;
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	ms_check_alloc(tmp, gcl);
	gc_register(tmp, gcl);
	cmd_path = ft_strjoin(tmp, command);
	ms_check_alloc(cmd_path, gcl);
	gc_register(cmd_path, gcl);
	return (cmd_path);
}
