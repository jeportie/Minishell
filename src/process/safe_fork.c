/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:27:43 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 18:48:48 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

/*
 * NOTE:
 * Take out getpid() before final push -> -42 function.
 */

pid_t	safe_fork(t_proc_manager *manager, t_fork_params *params)
{
	pid_t			pid;
	t_proc_params	new_params;

	if (!manager || !params)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: error: fork failed.\n");
		exit(EXIT_FAILURE);
	}
	new_params.pid = pid;
	new_params.parent_pid = getpid();
	new_params.child_lvl = params->child_lvl;
	new_params.fd_in = params->fd_in;
	new_params.fd_out = params->fd_out;
	new_params.fd_error = params->fd_error;
	new_params.is_heredoc = params->is_heredoc;
	new_params.title = params->title;
	if (pid > 0)
		add_process(manager, &new_params);
	return (pid);
}
