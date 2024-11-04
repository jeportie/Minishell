/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:22:43 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 13:23:07 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

/*static int	ft_print_env(t_env *env)
{
	t_env *(tmp) = env;
	while (tmp)
	{
		ft_dprintf(1, tmp->name_folder, "=", tmp->value_folder);
		ft_dprintf(1, "\n", NULL, NULL);
		tmp = tmp->next;
	}
	return (0);
}

static char	*ft_first_arg(char **strs)
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

static char	*ft_whithout_minus(char *str)
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

int	ft_env(t_infos *infos, t_tok *tmp)
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
