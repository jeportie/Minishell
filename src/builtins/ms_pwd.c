/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:24:12 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/05 09:50:29 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

int	ms_pwd(int fd)
{
	char	*pwd;
	int		error_w;

	error_w = 0;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_dprintf(2, "minishell: pwd: error retrieving current directory:"
			" getcwd: cannot access parent directories: No such file"
			" or directory\n");
		return (1);
	}
	error_w = ms_safe_putstr_fd(pwd, fd);
	if (error_w < 0)
	{
		ft_dprintf(2, "minishell: echo: write error:"
			" no space left on device\n");
		free(pwd);
		return (1);
	}
	ms_safe_putstr_fd("\n", fd);
	free(pwd);
	return (0);
}
