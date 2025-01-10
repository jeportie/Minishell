/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_operator_gestion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:39:00 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 13:34:27 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

static int	st_or_verif(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if ((synt->r_in || synt->r_here || synt->r_out || synt->r_app
			|| synt->o_and || synt->o_or || synt->o_pipe)
		|| (synt->p_trigger && synt->p_char == 0))
	{
		synt->error = 8;
		return (1);
	}
	else if (synt->p_trigger && synt->current[synt->i + 1]
		&& synt->current[synt->i + 1] == ')')
	{
		synt->p_trigger = 2;
		return (1);
	}
	else if (synt->i == 0)
		synt->error = 8;
	synt->i++;
	if (synt->i == synt->len_end)
		synt->error = 8;
	synt->o_or = 1;
	return (0);
}

static int	st_and_verif(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if ((synt->r_in || synt->r_here || synt->r_out || synt->r_app
			|| synt->o_and || synt->o_or || synt->o_pipe)
		|| (synt->p_trigger && synt->p_char == 0))
	{
		synt->error = 7;
		return (1);
	}
	else if (synt->p_trigger && synt->current[synt->i + 1]
		&& synt->current[synt->i + 1] == ')')
	{
		synt->p_trigger = 2;
		return (1);
	}
	else if (synt->i == 0)
		synt->error = 7;
	synt->i++;
	if (synt->i == synt->len_end)
		synt->error = 7;
	synt->o_and = 1;
	return (0);
}

static int	st_pipe_verif(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if ((synt->r_in || synt->r_here || synt->r_out || synt->r_app
			|| synt->o_and || synt->o_or || synt->o_pipe)
		|| (synt->p_trigger && synt->p_char == 0))
	{
		synt->error = 9;
		return (1);
	}
	else if (synt->p_trigger && synt->current[synt->i + 1]
		&& synt->current[synt->i + 1] == ')')
	{
		synt->p_trigger = 2;
		return (1);
	}
	else if (synt->i == 0)
		synt->error = 9;
	if (synt->i == synt->len_end)
		synt->error = 9;
	synt->o_pipe = 1;
	return (0);
}

int	ms_operator_gestion(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if (synt->current[synt->i] == '&' && synt->current[synt->i + 1]
		&& synt->current[synt->i + 1] == '&')
	{
		if (st_and_verif(&synt))
			return (1);
	}
	else if (synt->current[synt->i] == '|' && synt->current[synt->i + 1]
		&& synt->current[synt->i + 1] == '|')
	{
		if (st_or_verif(&synt))
			return (1);
	}
	else if (synt->current[synt->i] == '|')
	{
		if (st_pipe_verif(&synt))
			return (1);
	}
	return (0);
}
