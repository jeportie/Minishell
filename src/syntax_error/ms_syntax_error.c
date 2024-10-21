/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_syntax_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:11:22 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 13:09:14 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

int	ms_syntax_error(const char *input, t_shell *shell)
{
	char	*current;
	int		error;

	if (!input)
	{
		if (shell)
		{
			rl_clear_history();
			gc_cleanup(shell->gcl);
			free(shell->gcl);
		}
		perror("Minishell: Error: Segfault.\n");
		exit (EXIT_FAILURE);
	}
	if (!shell)
	{
		perror("Minishell: Error: Segfault.\n");
		exit (EXIT_FAILURE);
	}
	current = (char *)input;
	error = 0;
	error = ms_quote_error(current, shell);
	if (error == 0)
		error = ms_pipe_error(current, shell);
	if (error == 0)
		error = ms_operator_error(current, shell);
	return (error);
}
