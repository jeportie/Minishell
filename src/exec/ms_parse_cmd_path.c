/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_cmd_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:29:40 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 14:13:00 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/minishell.h"

char	*ms_parse_cmd_path(const char *command, t_shell *shell)
{
	char	**paths;
	char	*cmd_path;
	int		i;

	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		else
			return (NULL);
	}
	paths = ft_split(shell->env_data->path, ':');
	if (!paths)
	{
		perror("minishell: memory allocation error");
		gc_cleanup(shell->gcl);
		exit(EXIT_FAILURE);
	}
	gc_nest_register(paths, shell->gcl);
	gc_nest_lock(paths, shell->gcl);
	i = 0;
	cmd_path = NULL;
	while (paths[i])
	{
		cmd_path = ms_concat_path(paths[i], command, shell->gcl);
		if (!cmd_path)
		{
			gc_cleanup(shell->gcl);
			exit(EXIT_FAILURE);
		}
		if (access(cmd_path, X_OK) == 0)
		{
			return (cmd_path);
		}
		cmd_path = NULL;
		i++;
	}
	return (NULL);
}
