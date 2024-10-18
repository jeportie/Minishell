/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:28:50 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/18 13:36:01 by jeportie         ###   ########.fr       */
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

void	print_token(t_token *head)
{
	t_token	*current;
	int		i;

	current = head;
	i = 0;
	while (current)
	{
		printf("token %2d | type = %d\nvalue = %s\n", i,
			current->type, current->token);
		current = current->next;
		i++;
	}
}
