/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_value_assign.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:02:18 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/19 13:33:16 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include "../../include/minishell.h"

static	void	ms_copy_assign(char *var, char *value, char *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[i] != '=')
	{
		var[i] = cmd[i];
		i++;
	}
	var[i] = '\0';
	i++;
	j = 0;
	while (cmd[i])
	{
		value[j] = cmd[i];
		i++;
		j++;
	}
	value[j] = '\0';
}

static void	helper(t_cmd_node *cmd_node, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < cmd_node->argc)
	{
		if (is_var(cmd_node->argv[i]))
		{
			cmd_node->argv[i] = nested_vars(cmd_node->argv[i],
					shell);
		}
		i++;
	}
}

int	ms_value_assign(t_shell *shell, t_cmd_node *cmd_node, t_gc *gcl)
{
	char	*var;
	char	*value;
	int		len;

	int (i) = -1;
	i = -1;
	if (!cmd_node)
		return (-1);
	else
	{
		while (++i < cmd_node->argc)
		{
			len = valide_dollar(cmd_node->argv[i]);
			if (cmd_node->argv[i][len] == '=')
			{
				var = (char *)gc_malloc(sizeof(char) * (len + 1), gcl);
				len = ft_strlen(&cmd_node->argv[i][len]);
				value = (char *)gc_malloc(sizeof(char) * (len + 1), gcl);
				ms_copy_assign(var, value, cmd_node->argv[i]);
				ms_set_env_value(shell, var, value);
			}
		}
		helper(cmd_node, shell);
	}
	return (0);
}
