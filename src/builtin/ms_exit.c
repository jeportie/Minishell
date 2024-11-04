/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:23:11 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 13:23:35 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

/*int	ft_numeric_code(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static long long	ft_atoll_safe(const char *str, int *error)
{
	long long (result) = 0;
	long long (n_result) = 0;
	int (sign) = 1;
	int (i) = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		n_result = result * -1;
		if (!ft_isdigit(str[i]))
			return (*error = 1, 0);
		if (sign == 1 && result > (LLONG_MAX - (str[i] - '0')) / 10)
			return (*error = 1, LLONG_MAX);
		else if (sign == -1 && n_result < (LLONG_MIN + (str[i] - '0')) / 10)
			return (*error = 1, LLONG_MIN);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	ft_valid_numeric_code(char *str)
{
	int			error;
	long long	code;

	error = 0;
	code = ft_atoll_safe(str, &error);
	if (error || code > LONG_MAX || code < LONG_MIN)
		return (0);
	return (1);
}

int	ft_exit_bt(t_infos *infos, t_tok *tmp)
{
	int (code) = 0;
	if (!tmp->cmd[1])
		ft_quit(infos, 1, "exit\n", infos->code_error);
	if (tmp->cmd[1])
	{
		if (ft_numeric_code(tmp->cmd[1])
			&& ft_valid_numeric_code(tmp->cmd[1]))
			ft_quit(infos, 1, "exit\n", atol(tmp->cmd[1]) % 256);
		else
		{
			ft_dprintf(1, "exit\nminishell: exit: ", tmp->cmd[1],
				": numeric argument required\n");
			ft_quit(infos, 2, "", 2);
		}
	}
	return (code);
}*/
