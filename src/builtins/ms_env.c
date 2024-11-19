/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:22:43 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/19 14:12:25 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static int	st_print_env(t_env *env)
{
	t_env *(tmp) = env;
	while (tmp)
	{
		ft_dprintf(1, "export ", tmp->var);
		if (tmp->var)
			ft_dprintf(1, "%s", tmp->var);
		if (tmp->value)
			ft_dprintf(1, "=\"%s\"", tmp->value);
		ft_dprintf(1, "\n");
		tmp = tmp->next;
	}
	return (0);
}

static char	*st_first_arg(char **strs)
{
	int (i) = 1;
	int (j) = 0;
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
	int (i) = 0;
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
		return (st_print_env(env));
	if (cmd_node->argc == 2 && cmd_node->argv[1][0] == '-')
	{
		if (!cmd_node->argv[1][1])
			return (0);
		else if (cmd_node->argv[1][1] == '-')
		{
			if (!cmd_node->argv[1][2])
				return (st_print_env(env));
			else
				return (ft_dprintf(2, "env: unrecognized option %s\n'",
						cmd_node->argv[1]), 125);
		}
		else
		{
			ft_dprintf(2, "minishell: env: invalid option -- \'%s\'\n",
				st_whithout_minus(cmd_node->argv[1]));
			return (125);
		}
	}
	ft_dprintf(2, "minishell: env: ‘%s"
		"’: No such file or directory\n", st_first_arg(cmd_node->argv));
	return (127);
}

/*int	ft_env(t_infos *infos, t_tok *tmp)
{
	t_env *(env) = infos->tmp_env;
	if (!tmp->cmd[1])
		return (ft_print_env(env));
	if (tmp->cmd[1][0] == '-')
	{
		if (!tmp->cmd[1][1])
			return (0);
		else if (tmp->cmd[1][1] == '-')
		{
			if (!tmp->cmd[1][2])
				return (ft_print_env(env));
			else
				return (ft_dprintf(2, "env: unrecognized option '",
						tmp->cmd[1], "'\n"), 125);
		}
		else
		{
			ft_dprintf(2, "minishel: env: invalid option -- '",
				ft_whithout_minus(tmp->cmd[1]), "'\n");
			return (125);
		}
	}
	ft_dprintf(2, "minishell: env: ‘", ft_first_arg(tmp->cmd),
		"’: No such file or directory\n");
	return (127);
}*/
