/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:49:16 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 13:46:18 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void	st_move_token(t_token **top, t_token **prev, t_token **current)
{
	t_token	*swap;

	swap = (*current)->next;
	(*current)->next = (*top)->next;
	(*top)->next = *current;
	(*prev)->next = swap;
}

static void	st_init_sort_helper(t_sort_token_helper	*h, t_token *tokens)
{
	t_token	*current;

	current = tokens;
	h->current = tokens;
	h->head = h->current;
	h->top = h->head;
	h->prev = h->current;
	h->redir = 0;
	h->trigger = 0;
	h->final_path = 0;
	h->i = 0;
	h->len = 0;
	while (current)
	{
		h->len++;
		current = current->next;
	}
}

static int	st_check_token(t_sort_token_helper	*h)
{
	if (h->current->type == TOKEN_REDIRECTION && h->redir == 0)
	{
		h->redir = 1;
		h->trigger = 1;
	}
	else if (h->current->type == TOKEN_WORD || h->current->type == TOKEN_EXPAND)
	{
		if (h->redir == 1)
			h->redir = 0;
		else if (h->redir == 0 && h->top != h->current && h->trigger == 1)
		{
			st_move_token(&h->top, &h->prev, &h->current);
			h->top = h->current;
			h->current = h->prev;
			h->trigger = 0;
			h->final_path = 1;
		}
		else
			h->top = h->current;
	}
	else
		return (1);
	return (0);
}

static void	st_sort_token_list(t_token **tokens)
{
	t_sort_token_helper	h;

	st_init_sort_helper(&h, *tokens);
	while (h.current)
	{
		if (st_check_token(&h))
		{
			if (h.i < h.len - 1)
				h.top = h.current->next;
			h.trigger = 0;
		}
		h.i++;
		h.prev = h.current;
		if (h.current)
			h.current = h.current->next;
		if (!h.current && h.final_path == 1)
		{
			h.current = h.head;
			h.final_path = 0;
		}
	}
	*tokens = h.head;
}

t_token	*ms_tokenize(const char *command_line, t_gc *gcl)
{
	t_token *(new_token) = NULL;
	t_token *(tokens) = NULL;
	const char *(input) = command_line;
	ms_skip_whitespace(&input);
	while (*input)
	{
		new_token = NULL;
		if (ms_is_frame(*input))
			new_token = ms_tokenize_frame(&input, gcl);
		else if (ms_is_operator(*input))
			new_token = ms_tokenize_operator(&input, gcl);
		else
			new_token = ms_tokenize_word(&input, gcl);
		if (new_token)
			ms_add_token(&tokens, new_token, gcl);
		else
		{
			perror("Failed to tokenize input");
			gc_cleanup(gcl);
			exit(EXIT_FAILURE);
		}
		ms_skip_whitespace(&input);
	}
	st_sort_token_list(&tokens);
	return (tokens);
}
