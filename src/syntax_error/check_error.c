/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:12:29 by gmarquis          #+#    #+#             */
/*   Updated: 2024/12/03 13:29:14 by gmarquis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

int	find_close_par(char *current, int i)
{
	if (!current[i])
		return (0);
	while (current[i])
	{
		if (current[i] == ')')
			return (1);
		i++;
	}
	return (0);
}

static void	st_print_error_operator_redir(int error)
{
	if (error == 1)
		ft_dprintf(2, "`newline'\n");
	else if (error == 2)
		ft_dprintf(2, "`<'\n");
	else if (error == 3)
		ft_dprintf(2, "`<<'\n");
	else if (error == 4)
		ft_dprintf(2, "`>'\n");
	else if (error == 5)
		ft_dprintf(2, "`>>'\n");
	else if (error == 6)
		ft_dprintf(2, "`&'\n");
	else if (error == 7)
		ft_dprintf(2, "`&&'\n");
	else if (error == 8)
		ft_dprintf(2, "`||'\n");
	else if (error == 9)
		ft_dprintf(2, "`|'\n");
}

int	check_error(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if (synt->p_open != synt->p_close)
		synt->p_trigger = 2;
	if (synt->error == 10)
		synt->error = 1;
	else if (synt->p_trigger > 0 || synt->sq_trigger || synt->dq_trigger
		|| synt->error > 0)
	{
		ft_dprintf(2, "Minishell: syntax error near unexpected token ");
		if (synt->error > 0)
			st_print_error_operator_redir(synt->error);
		else if (synt->sq_trigger == 1)
			ft_dprintf(2, "`\''\n");
		else if (synt->dq_trigger == 1)
			ft_dprintf(2, "`\"'\n");
		else if (synt->p_trigger == 1)
			ft_dprintf(2, "`('\n");
		else if (synt->p_trigger == 2)
			ft_dprintf(2, "`)'\n");
		synt->error = 1;
	}
	return (synt->error);
}
