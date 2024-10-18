/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:39:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/18 14:37:12 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

void	add_token(t_token **head, t_token *new_token, t_gc *gcl)
{
	t_token	*current;

	if (!head)
	{
		gc_cleanup(gcl);
		perror("Minishell: Error: HEAD is NULL\n");
		exit (EXIT_FAILURE);
	}
	if (*head == NULL)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
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
