/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:49:16 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/18 15:43:38 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

t_token	*tokenize_quote(const char **input, t_gc *gcl)
{
	input = NULL;
	gcl = NULL;
	if (input == NULL && gcl == NULL)
		return (NULL);
	return (NULL);
}

t_token	*tokenize_operator(const char **input, t_gc *gcl)
{
	char	*value;
	int		len;
	int		i;
	char	*current;

	if (!input || !*input)
	{
		gc_cleanup(gcl);
		perror("Minishell: Error: null input\n");
		exit(EXIT_FAILURE);
	}
	current = (char *)*input;
	len = 1;
	i = 0;
	while (current[i] && current[i] == current[i + 1])
	{
		i++;
		len++;
	}
	value = (char *)gc_malloc(sizeof(char) * len + 1, gcl);
	gc_lock(value, gcl);
	i = 0;
	
	return (NULL);
}

t_token	*map_token_type_to_operator(const char *operator_token)
{
	operator_token = NULL;
	if (operator_token == NULL)
		return (NULL);
	return (NULL);
}

/*
 * NOTE: GERER SYNTAX ERROR QUAND TOKEN FAIL
 * ON init un garbage pour tokenize qu'on cleanup a la fin de l'ast
 */

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
