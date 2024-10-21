/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:41:20 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 16:09:52 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

/*
 * NOTE:
 *	if empty '' or "" token == ''
 */

t_token	*tokenize_word(const char **input, t_gc *gcl)
{
	char	*value;
	int		len;
	int		i;
	char	*current;
	bool	is_expand;

	if (!input || !*input)
	{
		gc_cleanup(gcl);
		perror("Minishell: Error: Null Input");
		exit(EXIT_FAILURE);
	}
	current = (char *)*input;
	len = 0;
	while (current[len] && !is_whitespace(current[len])
			&& !is_operator(current[len])// && !is_quote(current[len])
			&& !is_frame(current[len]))
		len++;
	value = (char *)gc_malloc(sizeof(char) * len + 1, gcl);
	gc_lock(value, gcl);
	i = 0;
	while (i < len)
	{
		value[i] = current[i];
		if (value[i] == '*' || value[i] == '$')
			is_expand = true;
		(*input)++;
		i++;
	}
	value[i] = '\0';
	if (is_expand == true)
		return (create_token(TOKEN_EXPAND, value, gcl));
	else
		return (create_token(TOKEN_WORD, value, gcl));
}
