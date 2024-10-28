/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_reinit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:18:39 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/28 16:42:49 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

static int	st_reinit_redir(char c)
{
	if (c != ' ' && c != '(' && c != ')' && c != '|'
		&& c != '<' && c != '>' && ft_isascii(c))
		return (1);
	return (0);
}

static int	st_reinit_operator(char c)
{
	if (c != ' ' && c != '(' && c != ')' && c != '|'
		&& ft_isascii(c))
		return (1);
	return (0);
}

static int	st_is_and(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if (synt->current[synt->i + 1] && synt->current[synt->i + 1] == '&'
		&& synt->o_and)
	{
		synt->error = 7;
		return (1);
	}
	if (synt->current[synt->i - 1] && synt->current[synt->i - 1] == '&'
		&& synt->o_and)
	{
		synt->error = 6;
		return (1);
	}
	return (0);
}

int	check_reinit(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if (synt->current[synt->i] == '&')
		if (st_is_and(&synt))
			return (1);
	if (st_reinit_redir(synt->current[synt->i]))
	{
		synt->r_in = 0;
		synt->r_here = 0;
		synt->r_out = 0;
		synt->r_app = 0;
	}
	if (st_reinit_operator(synt->current[synt->i]))
	{
		synt->o_and = 0;
		synt->o_or = 0;
		synt->o_pipe = 0;
	}
	if (!synt->dq_trigger && !synt->sq_trigger
		&& (synt->current[synt->i] == '<' || synt->current[synt->i] == '>'))
		if (redir_gestion(&synt))
			return (1);
	return (0);
}
