/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quote_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:02:50 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 13:03:46 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

int	ms_quote_error(const char *input, t_shell *shell)
{
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
	return (0);
}
