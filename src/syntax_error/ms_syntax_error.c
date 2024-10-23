/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_syntax_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:11:22 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/23 17:49:46 by jeportie         ###   ########.fr       */
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
	int		r_in;
	int		r_out;
	int		r_app;
	int		r_here;

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
	r_in = 0;
	r_out = 0;
	r_app = 0;
	r_here = 0;
	while (current[i])
	{
		if (current[i] != ' ' && current[i] != '(' && current[i] != ')' && current[i] != '&' && current[i] != '|' && current[i] != '<' && current[i] != '>'&& ft_isascii(current[i]))
		{
			r_in = 0;
			r_here = 0;
			r_out = 0;
			r_app = 0;
		}
		if (current[i] != ' ' && current[i] != '(' && current[i] != ')' && current[i] != '&' && current[i] != '|' && ft_isascii(current[i]))
		{
			o_and = 0;
			o_or = 0;
			o_pipe = 0;
		}
		if (current[i] == '&' && o_and)
		{
			error = 6;
			break ;
		}
		if (!dq_trigger && !sq_trigger && (current[i] == '<' || current[i] == '>'))
		{
			if (current[i] == '<')
			{
				if (current[i + 1] && current[i + 1] == '<')
				{
					if (r_in || r_here || r_out || r_app)
					{
						error = 3;
						break ;
					}
					i++;
					r_here = 1;
				}
				else
				{
					if (r_in || r_here || r_out || r_app)
					{
						error = 2;
						break ;
					}
					r_in = 1;
				}
			}
			else if (current[i] == '>')
			{
				if (current[i + 1] && current[i + 1] == '>')
				{
					if (r_in || r_here || r_out || r_app)
					{
						error = 5;
						break ;
					}
					i++;
					r_app = 1;
				}
				else
				{
					if (r_in || r_here || r_out || r_app)
					{
						error = 4;
						break ;
					}
					r_out = 1;
				}
			}
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
			else if (r_in || r_here || r_out || r_app)
			{
				p_trigger = 1;
				break ;
			}
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
			if (r_in || r_here || r_out || r_app)
			{
				p_trigger = 2;
				break ;
			}
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
			else if (o_and || o_or || o_pipe)
			{
				p_trigger = 2;
				break ;
			}
			else if (!dq_trigger && !sq_trigger && p_trigger && p_char > 0)
				p_trigger = 0;
		}
		else if (!dq_trigger && !sq_trigger && (current[i] == '&' || current[i] == '|'))
		{
			if (current[i] == '&' && current[i + 1] && current[i + 1] == '&')
			{
				if (r_in || r_here || r_out || r_app)
				{
					error = 7;
					break ;
				}
				if (o_and || o_or || o_pipe)
				{
					error = 7;
					break ;
				}
				if (i == 0)
					error = 7;
				i++;
				if (i == len_end)
					error = 7;
				o_and = 1;
			}
			else if (current[i] == '|' && current[i + 1] && current[i + 1] == '|')
			{
				if (r_in || r_here || r_out || r_app)
				{
					error = 8;
					break ;
				}
				if (o_and || o_or || o_pipe)
				{
					error = 8;
					break ;
				}
				if (i == 0)
					error = 8;
				i++;
				if (i == len_end)
					error = 8;
				o_or = 1;
			}
			else if (current[i] == '|')
			{
				if (r_in || r_here || r_out || r_app)
				{
					error = 9;
					break ;
				}
				if (o_and || o_or || o_pipe)
				{
					error = 9;
					break ;
				}
				if (i == 0)
					error = 9;
				if (i == len_end)
					error = 9;
				o_pipe = 1;
			}
		}
		else if (current[i] != ' ' && ft_isascii(current[i]) && p_trigger == 1)
			p_char++;
		i++;
	}
	if (p_trigger > 0 || sq_trigger || dq_trigger || error > 0)
	{
		if (error == 2)
			printf("Minishell: syntax error near unexpected token `<'\n");
		if (error == 3)
			printf("Minishell: syntax error near unexpected token `<<'\n");
		if (error == 4)
			printf("Minishell: syntax error near unexpected token `>'\n");
		if (error == 5)
			printf("Minishell: syntax error near unexpected token `>>'\n");
		if (error == 6)
			printf("Minishell: syntax error near unexpected token `&'\n");
		if (error == 7)
			printf("Minishell: syntax error near unexpected token `&&'\n");
		if (error == 8)
			printf("Minishell: syntax error near unexpected token `||'\n");
		if (error == 9)
			printf("Minishell: syntax error near unexpected token `|'\n");
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
