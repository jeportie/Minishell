/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:41:20 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/14 15:32:19 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

static void	handle_in_quote(t_copy_state *state)
{
	char	c;

	c = state->current[state->i];
	if (c == state->quote)
	{
		state->in_quote = false;
		state->quote = 0;
		state->i++;
	}
	else
		state->value[state->j++] = state->current[state->i++];
}

static void	handle_not_in_quote(t_copy_state *state)
{
	char	c;

	c = state->current[state->i];
	if (is_whitespace(c) || is_operator(c) || is_frame(c))
	{
		state->stop = true;
		return ;
	}
	else if (c == '\'' || c == '\"')
	{
		state->quote = c;
		state->in_quote = true;
		state->i++;
	}
	else
		state->value[state->j++] = state->current[state->i++];
}

static int	copy_token_value(char *value, const char *current, char start_quote)
{
	t_copy_state	state;

	state.current = current;
	state.value = value;
	state.i = 0;
	state.j = 0;
	state.in_quote = (start_quote != 0);
	state.quote = start_quote;
	state.stop = false;
	while (state.current[state.i] && !state.stop)
	{
		if (state.in_quote)
			handle_in_quote(&state);
		else
			handle_not_in_quote(&state);
	}
	state.value[state.j] = '\0';
	return (state.i);
}

t_token	*help_word(char *value, t_gc *gcl, bool is_expand)
{
	if (is_expand == true)
		return (create_token(TOKEN_EXPAND, value, gcl));
	else
		return (create_token(TOKEN_WORD, value, gcl));
}

t_token	*tokenize_word(const char **input, t_gc *gcl)
{
	char	*value;
	bool	is_expand;
	int		len;
	char	*current;

	char (quote) = 0;
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
	current = (char *)*input;
	is_expand = false;
	len = token_len(current, quote, &is_expand);
	value = (char *)gc_malloc(sizeof(char) * len + 1, gcl);
	gc_lock(value, gcl);
	len = copy_token_value(value, current, quote);
	*input += len;
	return (help_word(value, gcl, is_expand));
}
