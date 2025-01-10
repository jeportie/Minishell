/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quote_gestion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:29:03 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 13:35:52 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

int	ms_quote_gestion(t_syntax **syntax)
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
