/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_syntax_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:11:22 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/23 16:49:10 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"


static int	st_quote_error(const char *input)
{
	char	*current;
	int		error;
	int		i;
	int		len_end;
	int		sq_trigger;
	int		dq_trigger;
	int		p_trigger;
	int		p_char;
	int		o_and;
	int		o_or;
	int		o_pipe;

	current = ft_strtrim((char *)input, " ");
	len_end = ft_strlen(current) - 1;
	error = 0;
	i = 0;
	sq_trigger = 0;
	dq_trigger = 0;
	p_trigger = 0;
	p_char = 0;
	o_and = 0;
	o_or = 0;
	o_pipe = 0;
	while (current[i])
	{
		if (current[i] != ' ' && current[i] != '(' && current[i] != '&' && current[i] != '|' && ft_isascii(current[i]))
		{
			o_and = 0;
			o_or = 0;
			o_pipe = 0;
		}
		if (current[i] == '\'' && sq_trigger)
			sq_trigger = 0;
		else if (current[i] == '\"' && dq_trigger)
			dq_trigger = 0;
		else if (current[i] == '\'' && !dq_trigger)
			sq_trigger = 1;
		else if (current[i] == '\"' && !sq_trigger)
			dq_trigger = 1;
		else if (current[i] == '(' && !dq_trigger && !sq_trigger)
		{ 
			if (p_trigger == 1)
				break ;
			else if (i == 0 || o_and || o_or || o_pipe)
			{
				p_char = 0;
				p_trigger = 1;
				o_and = 0;
				o_or = 0;
				o_pipe = 0;
			}
			else if (i > 0 && !o_and && !o_or && !o_pipe)
			{
				p_trigger = 1;
				break ;
			}
		}
		else if (current[i] == ')')
		{
			if (!dq_trigger && !sq_trigger && p_trigger && p_char > 0)
				p_trigger = 0;
			else if (!dq_trigger && !sq_trigger && p_trigger && p_char == 0)
			{
				p_trigger = 2;
				break ;
			}
			else if (!dq_trigger && !sq_trigger && !p_trigger)
			{
				p_trigger = 2;
				break ;
			}
		}
		else if (!dq_trigger && !sq_trigger && (current[i] == '&' || current[i] == '|'))
		{
			if (current[i] == '&' && current[i + 1] && current[i + 1] == '&')
			{
				if (o_and || o_or || o_pipe)
				{
					error = 3;
					break ;
				}
				if (i == 0)
					error = 3;
				i++;
				printf("i = %d|end = %d\n", i, len_end);
				if (i == len_end)
					error = 3;
				o_and = 1;
			}
			else if (current[i] == '|' && current[i + 1] && current[i + 1] == '|')
			{
				if (o_and || o_or || o_pipe)
				{
					error = 3;
					break ;
				}
				if (i == 0)
					error = 3;
				i++;
				if (i == len_end)
					error = 3;
				o_or = 1;
			}
			else if (current[i] == '|')
			{
				if (o_and || o_or || o_pipe)
				{
					error = 3;
					break ;
				}
				if (i == 0)
					error = 3;
				if (i == len_end)
					error = 3;
				o_pipe = 1;
			}
		}
		else if (current[i] != ' ' && ft_isascii(current[i]) && p_trigger == 1)
			p_char++;
		i++;
	}
	if (p_trigger > 0 || sq_trigger || dq_trigger || error > 0)
	{
		if (error == 3)
			printf("Minishell: syntax error operator\n");
		error = 1;
		if (sq_trigger == 1)
			printf("Minishell: syntax error near unexpected token `\''\n");
		if (dq_trigger == 1)
			printf("Minishell: syntax error near unexpected token `\"'\n");
		if (p_trigger == 1)
			printf("Minishell: syntax error near unexpected token `('\n");
		if (p_trigger == 2)
			printf("Minishell: syntax error near unexpected token `)'\n");
	}
	return (error);
}

int	ms_syntax_error(const char *input)
{
	char	*current;
	int		error;

	current = (char *)input;
	error = 0;
	error = st_quote_error(current);
	return (error);
}
