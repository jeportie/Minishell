/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:22:43 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/07 12:40:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static int	st_print_env(t_env *env, int fd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->var && tmp->value)
		{
			if (ms_safe_putstr_fd(tmp->var, fd) < 0)
				return (1);
			if (ms_safe_putstr_fd("=", fd) < 0)
				return (1);
			if (ms_safe_putstr_fd(tmp->value, fd) < 0)
				return (1);
			if (ms_safe_putstr_fd("\n", fd) < 0)
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

static char	*st_first_arg(char **strs)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (strs[i])
	{
		j = 0;
		while (strs[i][j])
		{
			if (strs[i][j] != '-' || j > 1)
				return (strs[i]);
			j++;
		}
		i++;
	}
	return (strs[i - 1]);
}

static char	*st_whithout_minus(char *str)
{
	int	i;

	i = 0;
	while (str[i + 1])
	{
		str[i] = str[i + 1];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	ms_env(t_cmd_node *cmd_node, t_exec_context *context)
{
	t_env *(env) = context->shell->env_data->env;
	if (cmd_node->argc == 1)
		return (st_print_env(env, context->stdout_fd));
	if (cmd_node->argc == 2 && cmd_node->argv[1][0] == '-')
	{
		if (!cmd_node->argv[1][1])
			return (0);
		else if (cmd_node->argv[1][1] == '-')
		{
			if (!cmd_node->argv[1][2])
				return (st_print_env(env, context->stdout_fd));
			else
				return (ft_dprintf(2, "env: unrecognized option %s\n'",
						cmd_node->argv[1]), 125);
		}
		else
		{
			ft_dprintf(2, "env: invalid option -- \'%s\'\n",
				st_whithout_minus(cmd_node->argv[1]));
			return (125);
		}
	}
	ft_dprintf(2, "env: '%s"
		"': No such file or directory\n", st_first_arg(cmd_node->argv));
	return (127);
}
