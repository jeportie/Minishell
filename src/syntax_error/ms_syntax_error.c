/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_syntax_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:11:22 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 14:17:55 by jeportie         ###   ########.fr       */
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

int	ms_syntax_error(const char *input, t_shell *shell)
{
	char	*current;
	int		error;

	st_args_verif(input, shell);
	current = (char *)input;
	error = 0;
	error = ms_quote_error(current, shell);
	if (error == 0)
		error = ms_pipe_error(current, shell);
	if (error == 0)
		error = ms_operator_error(current, shell);
	return (error);
}
