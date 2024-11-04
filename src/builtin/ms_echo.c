/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:21:58 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 13:22:37 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

/*static int	ft_is_n(char *str)
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

static int	ft_echo_utils(int i, int j, char **cmd, int *error)
{
	int	error_w;

	error_w = 0;
	while (cmd[j][i])
	{
		if (cmd[j][i] != '"')
			error_w = write(1, &cmd[j][i], 1);
		if (error_w < 0)
		{
			ft_dprintf(2, "minishell: echo: write error:"
				" no space left on device\n", NULL, NULL);
			*error = 1;
			return (1);
		}
		i++;
	}
	if (cmd[j + 1])
		write(1, " ", 1);
	return (i);
}

int	ft_echo(t_tok *tmp)
{
	int (i) = 0;
	int (trigger) = 0;
	int (flag) = 0;
	int (j) = 0;
	int (error)[] = {0};
	if (tmp->cmd[1])
	{
		while (tmp->cmd[++j])
		{
			i = 0;
			if (trigger < 2 && ft_is_n(tmp->cmd[j]))
			{
				flag++;
				trigger = 1;
			}
			else
			{
				trigger = 2;
				i = ft_echo_utils(i, j, tmp->cmd, error);
			}
		}
	}
	if (flag == 0 && *error == 0)
		ft_dprintf(1, "\n", NULL, NULL);
	return (0);
}*/
