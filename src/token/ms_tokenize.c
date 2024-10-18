/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:49:16 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/18 12:57:42 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

void	print_token(t_token *head)
{
	t_token	*current;
	int		i;

	current = head;
	i = 0;
	while (current)
	{
		printf("token %2d | type = %d\nvalue = %s\n", i,
			current->type, current->token);
		current = current->next;
		i++;
	}
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	current = *head;
	while (current)
		current = current->next;
	current = new_token;
	return ;
}

t_token	*create_token(t_token_type type, const char *value, t_gc *gcl)
{
	t_token	*new_token;

	new_token = (t_token *)gc_malloc(sizeof(t_token), gcl);
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->type = type;
	new_token->token = (char *)value;
	return (new_token);
}

t_token	*tokenize_word(const char **input, t_gc *gcl)
{
	char	*value;
	int		len;
	int		i;
	char	*current;

	current = (char *)*input;
	len = 0;
	while (current[len] && !is_whitespace(current[len]))
		len++;
	value = (char *)gc_malloc(sizeof(char) * len + 1, gcl);
	gc_lock(value, gcl);
	i = 0;
	while (current[i] && !is_whitespace(current[i]))
	{
		value[i] = current[i];
		(*input)++;
		i++;
	}
	value[i] = '\0';
	return (create_token(TOKEN_WORD, value, gcl));
}

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
	input = NULL;
	gcl = NULL;
	if (input == NULL && gcl == NULL)
		return (NULL);
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
