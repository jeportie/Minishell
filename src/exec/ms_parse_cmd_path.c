/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_cmd_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:29:40 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/01 14:22:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/minishell.h"

char	*ms_parse_cmd_path(const char *command, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*cmd_path;
	int		i;

	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup_gc(command));
		else
			return (NULL);
	}
	path_env = (char *)ms_getenv("PATH", shell->env_data);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		perror("minishell: memory allocation error");
		gc_cleanup();
		exit(EXIT_FAILURE);
	}
	gc_nest_register(paths);
	gc_nest_lock(paths);
	i = 0;
	cmd_path = NULL;
	while (paths[i])
	{
		cmd_path = ms_concat_path(paths[i], command);
		if (!cmd_path)
		{
			gc_cleanup();
			exit(EXIT_FAILURE);
		}
		if (access(cmd_path, X_OK) == 0)
		{
			gc_nest_unlock(paths);
			gc_collect();
			return (cmd_path);
		}
		gc_unlock(cmd_path);
		gc_collect();
		cmd_path = NULL;
		i++;
	}
	gc_nest_unlock(paths);
	gc_collect();
	return (NULL);
}
