/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:52:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/15 12:59:14 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 *NOTE: Step 1: 
 *->Files creation
 *->structure 
 *->open minishell and display a prompt
 *->first test unit for main and diplsay function
 *->Coding + Test green
 *->Refactoring
 *->Next Test coding
 *->End step one
 */

#include "include/minishell.h"

int	g_signal;

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
		printf("user_input : %s\n", shell.user_input);
		shell.error_code = 0;
	}
	shell.error_code = 666;
	return (shell.error_code);
}
