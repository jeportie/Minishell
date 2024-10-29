/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:52:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/29 19:32:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"
#include "include/syntax.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	shell = ms_init_shell(argc, argv, envp);
	while (1)
	{
		ms_init_std_signal();
		ms_get_user_input(&shell);
		if (!shell.user_input)
		{
			rl_clear_history();
			gc_cleanup(shell.gcl);
			free(shell.gcl);
			exit (shell.error_code);
		}
		if (ms_syntax_error(shell.user_input))
			shell.error_code = 2;
		else
		{
			printf("user_input : %s\n", shell.user_input);
			shell.error_code = 0;
		}
	}
	shell.error_code = 666;
	return (shell.error_code);
}
