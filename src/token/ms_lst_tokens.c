/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_lst_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:39:47 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 13:45:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"
#include <fcntl.h>

void	ms_add_token(t_token **head, t_token *new_token, t_gc *gcl)
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

t_token	*ms_create_token(t_token_type type, const char *value, t_gc *gcl)
{
	t_token	*new_token;

	new_token = (t_token *)gc_malloc(sizeof(t_token), gcl);
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->type = type;
	new_token->token = (char *)value;
	return (new_token);
}

static void	st_print_token(t_token *head)
{
	t_token	*current;
	int		i;
	int		fd;

	current = head;
	i = 0;
	fd = open(PRINT_INFOS, O_WRONLY | O_APPEND, COPY_MODE);
	while (current)
	{
		ft_dprintf(fd, "token %2d | type = %d\nvalue = %s.\n", i,
			current->type, current->token);
		current = current->next;
		i++;
	}
	close(fd);
}

void	ms_print_token_delimit(t_token *tokens)
{
	int	fd;

	if (DEBUG == 0)
		return ;
	fd = open(PRINT_INFOS, O_WRONLY | O_TRUNC | O_CREAT, COPY_MODE);
	ft_dprintf(fd, "-----------------------------------------------------");
	ft_dprintf(fd, "---------------------------\nTOKENS:\n");
	st_print_token(tokens);
	close(fd);
}
