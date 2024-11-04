/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:24:12 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 13:36:19 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

int	ms_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_dprintf(2, "minishell: pwd: error retrieving current directory:"
			" getcwd: cannot access parent directories: No such file"
			" or directory\n");
		return (1);
	}
	ft_dprintf(1, "%s\n", pwd);
	free(pwd);
	return (0);
}

/*int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_dprintf(2, "minishell: pwd: error retrieving current directory:"
			" getcwd: cannot access parent directories: No such file"
			" or directory\n", NULL, NULL);
		return (1);
	}
	ft_dprintf(1, pwd, "\n", NULL);
	pwd = ft_free_str(pwd);
	return (0);
}*/
