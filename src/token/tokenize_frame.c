/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_frame.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:40:25 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/26 16:21:31 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

t_token	*tokenize_frame(const char **input, t_gc *gcl)
{
	char	*value;
	char	*current;

	if (!input || !*input)
	{
		gc_cleanup(gcl);
		perror("Minishell: Error: Null Input");
		exit(EXIT_FAILURE);
	}
	current = (char *)*input;
	value = (char *)gc_malloc(sizeof(char) * 2, gcl);
	value[0] = current[0];
	value[1] = '\0';
	(*input)++;
	if (value[0] == '(')
		return (create_token(TOKEN_SUBSHELL_START, value, gcl));
	else
		return (create_token(TOKEN_SUBSHELL_STOP, value, gcl));
}
