/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redirect_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:57:40 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 10:57:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	ms_redirect_input(int in_fd)
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 error (input)");
		exit(EXIT_FAILURE);
	}
	close(in_fd);
}

void	ms_redirect_output(int out_fd)
{
	if (dup2(out_fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 error (output)");
		exit(EXIT_FAILURE);
	}
	close(out_fd);
}
