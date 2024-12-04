/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:21:58 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/04 13:01:56 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static int	st_is_n(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-')
		i++;
	else
		return (0);
	if (!str[i])
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

static void	st_echo_utils(t_echo_utils *utils, char **cmd, int fd)
{
	if (cmd[utils->i])
		if (ms_safe_putstr_fd(cmd[utils->i], fd) < 0)
			utils->error = 1;
	if (cmd[utils->i + 1])
		if (ms_safe_putstr_fd(" ", fd) < 0)
			utils->error = 1;
}

int	ms_echo(t_cmd_node *cmd_node, int fd)
{
	t_echo_utils	utils;

	ft_memset(&utils, 0, sizeof(t_echo_utils));
	if (cmd_node->argc > 1)
	{
		while (cmd_node->argv[++utils.i])
		{
			if (utils.trigger < 2 && st_is_n(cmd_node->argv[utils.i]))
			{
				utils.flag++;
				utils.trigger = 1;
			}
			else
			{
				utils.trigger = 2;
				st_echo_utils(&utils, cmd_node->argv, fd);
			}
		}
	}
	if (utils.flag == 0 && utils.error == 0)
		if (ms_safe_putstr_fd("\n", fd) < 0)
			return (utils.error = 1, 1);
	return (utils.error);
}
