/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_gestion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:29:03 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/24 16:31:04 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

int	quote_gestion(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if (synt->current[synt->i] == '\'' && synt->sq_trigger)
	{
		synt->sq_trigger = 0;
		return (1);
	}
	else if (synt->current[synt->i] == '\"' && synt->dq_trigger)
	{
		synt->dq_trigger = 0;
		return (1);
	}
	else if (synt->current[synt->i] == '\'' && !synt->dq_trigger)
	{
		synt->sq_trigger = 1;
		return (1);
	}
	else if (synt->current[synt->i] == '\"' && !synt->sq_trigger)
	{
		synt->dq_trigger = 1;
		return (1);
	}
	return (0);
}
