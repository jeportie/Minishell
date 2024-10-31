/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 08:34:30 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/31 12:54:46 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

static void	initialize_token_state(t_token_state *state, const char *current,
	char start_quote)
{
	state->current = current;
	state->i = 0;
	state->len = 0;
	state->is_expand = false;
	state->quote = start_quote;
	state->in_quote = (start_quote != 0);
	state->start_quote = start_quote;
	state->stop = false;
}

static void	handle_in_quote(t_token_state *state)
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
	{
		if (state->start_quote == '"' && (c == '*' || c == '$'))
			state->is_expand = true;
		state->len++;
		state->i++;
	}
}

static void	handle_not_in_quote(t_token_state *state)
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
	{
		if ((state->start_quote == '"' || state->start_quote == 0)
			&& (c == '*' || c == '$'))
			state->is_expand = true;
		state->len++;
		state->i++;
	}
}

int	token_len(const char *current, char start_quote, bool *is_expand)
{
	t_token_state	state;

	initialize_token_state(&state, current, start_quote);
	while (state.current[state.i] && !state.stop)
	{
		if (state.in_quote)
			handle_in_quote(&state);
		else
			handle_not_in_quote(&state);
	}
	*is_expand = state.is_expand;
	return (state.len);
}
