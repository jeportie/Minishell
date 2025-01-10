/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtins_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:47:09 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 13:54:04 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

int	ms_safe_putstr_fd(char *s, int fd)
{
	size_t	len;
	int		error;

	if (!fd || !s)
		return (1);
	error = 0;
	len = ft_strlen(s);
	error = write(fd, s, len);
	if (error < 0)
	{
		write(2, SHELL, ft_strlen(SHELL));
		write(2, ": write error: ", 15);
		write(2, "no space left on device\n", 24);
		return (1);
	}
	return (0);
}

int	ms_valide_var(char *input)
{
	int (i) = 0;
	if (ft_isdigit(input[i]))
	{
		ft_dprintf(2, SHELL ": export: `%s\':"
			" not a valid identifier\n", input);
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
			ft_dprintf(2, SHELL ": export: `%s\':"
				" not a valid identifier\n", input);
			return (0);
		}
	}
	return (i);
}
