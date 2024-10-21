/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quote_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:02:50 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 15:39:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

static void	st_args_verif(const char *input, t_shell *shell)
{
	int	error;

	error = 0;
	if (!input || !*input)
	{
		if (shell)
		{
			rl_clear_history();
			gc_cleanup(shell->gcl);
			free(shell->gcl);
		}
		error = 1;
	}
	if (!shell)
		error = 1;
	if (error)
	{
		perror("Minishell: Error: Segfault.\n");
		exit (EXIT_FAILURE);
	}
}

static int	st_matching_quote(char *current)
{
	return (0);
}

int	ms_quote_error(const char *input, t_shell *shell)
{
	int		error;
	char	*current;

	st_args_verif(input, shell);
	current = (char *)input;
	error = 0;
	if (ft_strchr(current, '\'') || ft_strchr(current, '\"'))
		error = st_matching_quote(current);
	return (error);
}
