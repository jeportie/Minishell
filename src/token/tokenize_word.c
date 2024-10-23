/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:41:20 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/23 15:57:39 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

static int	token_len(const char *current, char start_quote, bool *is_expand)
{
	char	quote;
	bool	in_quote;
	int		i;
	int		len;

	i = 0;
	if (start_quote != 0)
		in_quote = true;
	else
		in_quote = false;
	*is_expand = false;
	quote = start_quote;
	len = 0;
	while (current[i])
	{
		if (in_quote)
		{
			if (current[i] == quote)
			{
				in_quote = false;
				quote = 0;
				i++;
			}
			else
			{
				if (start_quote == '\"' && (current[i] == '*'
						|| current[i] == '$'))
					*is_expand = true;
				len++;
				i++;
			}
		}
		else
		{
			if (is_whitespace(current[i]) || is_operator(current[i])
				|| is_frame(current[i]))
				break ;
			else if (current[i] == '\'' || current[i] == '\"')
			{
				quote = current[i];
				in_quote = true;
				i++;
			}
			else
			{
				if ((start_quote == '\"' || start_quote == 0) && (current[i] == '*'
						|| current[i] == '$'))
					*is_expand = true;
				len++;
				i++;
			}
		}
	}
	return (len);
}

static int	copy_token_value(char *value, const char *current, char start_quote)
{
	int		i;
	int		j;
	char	quote;
	bool	in_quote;

	i = 0;
	j = 0;
	if (start_quote != 0)
		in_quote = true;
	else
		in_quote = false;
	quote = start_quote;
	while (current[i])
	{
		if (in_quote)
		{
			if (current[i] == quote)
			{
				in_quote = false;
				quote = 0;
				i++;
			}
			else
			{
				value[j++] = current[i++];
			}
		}
		else
		{
			if (is_whitespace(current[i]) || is_operator(current[i])
				|| is_frame(current[i]))
				break ;
			else if (current[i] == '\'' || current[i] == '\"')
			{
				quote = current[i];
				in_quote = true;
				i++;
			}
			else
			{
				value[j++] = current[i++];
			}
		}
	}
	value[j] = '\0';
	return (i);
}

t_token	*tokenize_word(const char **input, t_gc *gcl)
{
	char	*value;
	char	quote;
	bool	is_expand;
	int		len;
	char	*current;

	if (!input || !*input)
	{
		gc_cleanup(gcl);
		perror("Minishell: Error: Null Input");
		exit(EXIT_FAILURE);
	}
	if (**input == '\'' || **input == '\"')
	{
		quote = **input;
		(*input)++;
	}
	else
		quote = 0;
	current = (char *)*input;
	is_expand = false;
	len = token_len(current, quote, &is_expand);
	value = (char *)gc_malloc(sizeof(char) * len + 1, gcl);
	gc_lock(value, gcl);
	len = copy_token_value(value, current, quote);
	*input += len;
	if (is_expand == true)
		return (create_token(TOKEN_EXPAND, value, gcl));
	else
		return (create_token(TOKEN_WORD, value, gcl));
}
