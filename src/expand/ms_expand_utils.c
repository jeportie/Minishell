/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 08:52:30 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/19 13:49:29 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"

bool	is_var(char *cmd)
{
	int	i;

	if (!cmd)
		return (false);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

bool	is_wild(char *cmd)
{
	int	i;

	if (!cmd)
		return (false);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '*')
			return (true);
		i++;
	}
	return (false);
}

bool	is_equal(char *cmd)
{
	int	i;

	if (!cmd)
		return (false);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

int	valide_dollar(char *input)
{
	if (!input)
	{
		ft_dprintf(2, "minishell: %s\': no input\n", input);
		return (0);
	}
	int (i) = 0;
	if (ft_isdigit(input[i]))
	{
		ft_dprintf(2, "minishell: %s\': command not found\n", input);
		return (0);
	}
	while (input[i])
	{
		if (ft_isalnum(input[i]) || input[i] == '_')
			i++;
		else if ((input[i] == '=') || (input[i] == '+' && input[i + 1] == '='))
			return (i);
		else
		{
			ft_dprintf(2, "minishell: %s\':command not found\n", input);
			return (0);
		}
	}
	return (i);
}

char	*find_dollar(char *arg)
{
	int		i;

	if (!arg)
		return (NULL);
	i = 0;
	while (arg[i] && arg[i] != '$')
		i++;
	if (arg[i] != '$')
		return (NULL);
	i++;
	return (&arg[i]);
}
