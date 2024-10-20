/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:52:27 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/20 18:37:35 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

/*
 * NOTE:
 * |  = PIPE_TOKEN
 * || = OR_TOKEN
 * && = AND_TOKEN
 * <> = REDIRECTION_TOKEN
 * << = REDIRECTION_TOKEN
 * >> = REDIRECTION_TOKEN
 * *  = WILDCARD_TOKEN
 * ?  = WILDCARD_TOKEN
 * $  = STATUS_TOKEN 
 */

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
