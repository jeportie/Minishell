/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:28:50 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/08 20:40:38 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	is_operator(char c)
{
	return (c == '|' || c == '&' || c == ';' || c == '<' || c == '>'
		|| c == '(' || c == ')');
}

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

bool	is_special(char c)
{
	return (c == '*' || c == '?' || c == '[' || c == ']'
		|| c == '{' || c == '}' || c == '~' || c == '$'
		|| c == '`' || c == '\\');
}

void	skip_whitespace(const char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
}
