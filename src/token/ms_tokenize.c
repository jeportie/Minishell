/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:49:16 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/02 16:25:20 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

t_token	*ms_tokenize(const char *command_line, t_gc *gcl)
{
	t_token		*new_token;

	t_token *(tokens) = NULL;
	const char *(input) = command_line;
	skip_whitespace(&input);
	while (*input)
	{
		new_token = NULL;
		if (is_frame(*input))
			new_token = tokenize_frame(&input, gcl);
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

void	print_token_delimit(t_token *tokens)
{
	int	fd;

	if (DEBUG == 0)
		return ;
	fd = open(PRINT_INFOS, O_WRONLY | O_TRUNC | O_CREAT, COPY_MODE);
	ft_dprintf(fd, "-----------------------------------------------------");
	ft_dprintf(fd, "---------------------------\nTOKENS:\n");
	print_token(tokens);
	close(fd);
}
