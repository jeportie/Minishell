/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_syntax_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student->42->fr>          +#+  +:+       +#+      */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:11:22 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/24 16:31:16 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

static int	st_line_check(t_syntax *synt)
{
	while (synt->current[synt->i])
	{
		if (check_reinit(&synt))
			break ;
		if (quote_gestion(&synt))
			;
		else if ((synt->current[synt->i] == '(' && !synt->dq_trigger
				&& !synt->sq_trigger) || (synt->current[synt->i] == ')'))
		{
			if (parenthesis_gestion(&synt))
				break ;
		}
		else if (!synt->dq_trigger && !synt->sq_trigger
			&& (synt->current[synt->i] == '&' || synt->current[synt->i] == '|'))
		{
			if (operator_gestion(&synt))
				break ;
		}
		else if (synt->current[synt->i] != ' '
			&& ft_isascii(synt->current[synt->i]) && synt->p_trigger == 1)
			synt->p_char++;
		synt->i++;
	}
	return (check_error(&synt));
}

int	ms_syntax_error(const char *input)
{
	t_syntax	synt;
	int			error;

	ft_memset(&synt, 0, sizeof(t_syntax));
	synt.current = ft_strtrim((char *)input, " ");
	synt.len_end = ft_strlen(synt.current) - 1;
	error = st_line_check(&synt);
	return (error);
}
