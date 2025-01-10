/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize_word_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 08:34:30 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 13:50:06 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

static void	st_initialize_token_state(t_token_state *state, const char *current,
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

static void	st_handle_in_quote(t_token_state *state)
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

static void	st_handle_not_in_quote(t_token_state *state)
{
	char	c;

	c = state->current[state->i];
	if (ms_is_whitespace(c) || ms_is_operator(c) || ms_is_frame(c))
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

int	ms_token_len(const char *current, char start_quote, bool *is_expand)
{
	t_token_state	state;

	st_initialize_token_state(&state, current, start_quote);
	while (state.current[state.i] && !state.stop)
	{
		if (state.in_quote)
			st_handle_in_quote(&state);
		else
			st_handle_not_in_quote(&state);
	}
	*is_expand = state.is_expand;
	return (state.len);
}
