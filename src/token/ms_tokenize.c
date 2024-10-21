/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:49:16 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 16:58:26 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

void	add_token(t_token **head, t_token *new_token)
{
	if (head || new_token)
		return ;
	return ;
}

t_token	*tokenize_word(const char **input, t_gc *gcl)
{
	if (input || gcl)
		return (NULL);
	return (NULL);
}

t_token	*tokenize_quote(const char **input, t_gc *gcl)
{
	if (input || gcl)
		return (NULL);
	return (NULL);
}

t_token	*tokenize_operator(const char **input, t_gc *gcl)
{
	if (input || gcl)
		return (NULL);
	return (NULL);
}

t_token	*map_token_type_to_operator(const char *operator_token)
{
	if (operator_token)
		return (NULL);
	return (NULL);
}

t_token	*ms_tokenize(const char *command_line, t_gc *gcl)
{
	t_token		*tokens;
	t_token		*new_token;
	const char	*input = command_line;

	tokens = NULL;
	skip_whitespace(&input);
	while (*input)
	{
		new_token = NULL;
		if (is_quote(*input))
			new_token = tokenize_quote(&input, gcl);
		else if (is_operator(*input))
			new_token = tokenize_operator(&input, gcl);
		else
			new_token = tokenize_word(&input, gcl);
		if (new_token)
			add_token(&tokens, new_token);
		else
		{
			perror("Failed to tokenize input");
			gc_cleanup(gcl);
			exit(EXIT_FAILURE);
		}
		skip_whitespace(&input);
	}
	return (tokens);
}
