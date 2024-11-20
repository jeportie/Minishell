/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:23:11 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/19 11:21:58 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static int	st_numeric_code(char *str)
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

static long long	st_atoll_safe(const char *str, int *error)
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

static int	st_valid_numeric_code(char *str)
{
	int			error;
	long long	code;

	error = 0;
	code = st_atoll_safe(str, &error);
	if (error || code > LONG_MAX || code < LONG_MIN)
		return (0);
	return (1);
}

static void	st_exit_code(t_cmd_node *cmd_node, t_exec_context *context)
{
	long long	code;

	if (st_numeric_code(cmd_node->argv[1])
		&& st_valid_numeric_code(cmd_node->argv[1]))
	{
		ft_dprintf(2, "exit\n");
		code = ft_atol(cmd_node->argv[1]);
		gc_cleanup(context->shell->gcl);
		exit (code % 256);
	}
	else
	{
		ft_dprintf(2, "exit\nminishell: exit: %s"
			": numeric argument required\n", cmd_node->argv[1]);
		gc_cleanup(context->shell->gcl);
		exit (2);
	}
}

int	ms_exit(t_cmd_node *cmd_node, t_exec_context *context)
{
	if (cmd_node->argc == 1)
	{
		ft_dprintf(2, "exit\n");
		gc_cleanup(context->shell->gcl);
		exit (context->shell->error_code);
	}
	else if (cmd_node->argc == 2)
		st_exit_code(cmd_node, context);
	ft_dprintf(2, "exit\nminishell: exit: too many arguments\n");
	return (1);
}
