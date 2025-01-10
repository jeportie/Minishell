/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize_frame.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:40:25 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 13:46:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

t_token	*ms_tokenize_frame(const char **input, t_gc *gcl)
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
		return (ms_create_token(TOKEN_SUBSHELL_START, value, gcl));
	else
		return (ms_create_token(TOKEN_SUBSHELL_STOP, value, gcl));
}
