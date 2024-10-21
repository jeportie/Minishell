/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:49:16 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 14:11:57 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

t_token	*ms_tokenize(const char *command_line, t_gc *gcl)
{
	t_token		*tokens;
	t_token		*new_token;
	const char	*input;

	input = command_line;
	tokens = NULL;
	skip_whitespace(&input);
	while (*input)
	{
		new_token = NULL;
		if (is_quote(*input))
			new_token = tokenize_quote(&input, gcl);
		else if (is_frame(*input))
			new_token = tokenize_frame(&input, gcl);
		else if (is_operator(*input))
			new_token = tokenize_operator(&input, gcl);
		else
			new_token = tokenize_word(&input, gcl);
		if (new_token)
			add_token(&tokens, new_token, gcl);
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
