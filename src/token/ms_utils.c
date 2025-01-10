/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:28:50 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 13:50:27 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

bool	ms_is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	ms_is_operator(char c)
{
	return (c == '|' || c == '&' || c == '<' || c == '>');
}

bool	ms_is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

bool	ms_is_frame(char c)
{
	return (c == '(' || c == ')');
}

void	ms_skip_whitespace(const char **input)
{
	while (**input && ms_is_whitespace(**input))
		(*input)++;
}
