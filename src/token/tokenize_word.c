/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:41:20 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/18 14:49:00 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

t_token	*tokenize_word(const char **input, t_gc *gcl)
{
	char	*value;
	int		len;
	int		i;
	char	*current;

	if (!input || !*input)
	{
		gc_cleanup(gcl);
		perror("Minishell: Error: Null Input");
		exit(EXIT_FAILURE);
	}
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
