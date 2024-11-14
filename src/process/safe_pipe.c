/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:49:58 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 15:05:50 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/process.h"

int	safe_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: error: pipe failed\n");
		exit(EXIT_FAILURE);
	}
	return (0);
}