/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:11:16 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 15:32:57 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

t_token	*tokenize_quote(const char **input, t_gc *gcl)
{
	char	*value;
	char	quote;
	bool	is_expand;
	int		len;
	int		i;
	char	*current;

	if (!input || !*input)
	{
		gc_cleanup(gcl);
		perror("Minishell: Error: Null Input");
		exit(EXIT_FAILURE);
	}
	quote = **input;
	(*input)++;
	current = (char *)*input;
	len = 0;
	while (current[len] && current[len] != quote)
		len++;
	if (!current[len])
	{
		gc_cleanup(gcl);
		perror("Minishell: Syntax Error: Closing quote missing!\n");
		exit(EXIT_FAILURE);
	}
	value = (char *)gc_malloc(sizeof(char) * len + 1, gcl);
	gc_lock(value, gcl);
	i = 0;
	while (i < len)
	{
		value[i] = current[i];
		if (quote == '\"' && (value[i] == '*' || value[i] == '$'))
			is_expand = true;
		(*input)++;
		i++;
	}
	(*input)++;
	value[i] = '\0';
	if (quote == '\"' && is_expand == true)
		return (create_token(TOKEN_EXPAND, value, gcl));
	else
		return (create_token(TOKEN_WORD, value, gcl));
}
