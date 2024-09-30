/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:52:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/09/30 13:46:09 by jeportie         ###   ########.fr       */
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

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	char *test;

	test = gc_malloc(sizeof(char) * 5);
	test[0] = 'a';
	gc_lock(test);
	gc_collect();

	(void)argv;
	shell = ms_init_shell(argc, envp);
	while (1)
	{
		ms_get_user_input(&shell);
		if (!shell.user_input)
		{
			rl_clear_history();
			gc_cleanup();
			exit (shell.error_code);
		}
		printf("user_input : %s\n", shell.user_input);
	}
	shell.error_code = 666;
	return (shell.error_code);
}
