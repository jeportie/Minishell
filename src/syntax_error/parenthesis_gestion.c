/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_gestion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:44:36 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/24 16:28:05 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

static int	st_close_parenthesis(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if (synt->r_in || synt->r_here || synt->r_out || synt->r_app)
		return (synt->p_trigger = 2, 1);
	else if (!synt->dq_trigger && !synt->sq_trigger && synt->p_trigger
		&& synt->p_char == 0)
		return (synt->p_trigger = 2, 1);
	else if (!synt->dq_trigger && !synt->sq_trigger && !synt->p_trigger)
		return (synt->p_trigger = 2, 1);
	else if (synt->o_and || synt->o_or || synt->o_pipe)
		return (synt->p_trigger = 2, 1);
	else if (!synt->dq_trigger && !synt->sq_trigger && synt->p_trigger
		&& synt->p_char > 0)
		synt->p_trigger = 0;
	return (0);
}

static int	st_open_parenthesis(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if (synt->p_trigger == 1)
		return (1);
	else if (synt->r_in || synt->r_here || synt->r_out || synt->r_app)
	{
		synt->p_trigger = 1;
		return (1);
	}
	else if (synt->i == 0 || synt->o_and || synt->o_or || synt->o_pipe)
	{
		synt->p_char = 0;
		synt->p_trigger = 1;
		synt->o_and = 0;
		synt->o_or = 0;
		synt->o_pipe = 0;
	}
	else if (synt->i > 0 && !synt->o_and && !synt->o_or
		&& !synt->o_pipe)
	{
		synt->p_trigger = 1;
		return (1);
	}
	return (0);
}

int	parenthesis_gestion(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if (synt->current[synt->i] == '(' && !synt->dq_trigger
		&& !synt->sq_trigger)
	{
		if (st_open_parenthesis(&synt))
			return (1);
	}
	else if (synt->current[synt->i] == ')')
	{
		if (st_close_parenthesis(&synt))
			return (1);
	}
	return (0);
}
