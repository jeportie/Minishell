/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:52:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/14 15:52:00 by jeportie         ###   ########.fr       */
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

static void	ts_aff_env(t_env_data *env_data)
{
	t_env	*current;

	current = env_data->env;
	printf("fantom_path = %s\n\n", env_data->path);
	while (current)
	{
		printf("minishell = %s = %s\n", current->var, current->value);
		current = current->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	shell = ms_init_shell(argc, argv, envp);
	while (1)
	{
		ms_get_user_input(&shell);
		if (!ft_strncmp(shell.user_input, "env", 4))
			ts_aff_env(shell.env_data);
		if (!shell.user_input)
		{
			rl_clear_history();
			gc_cleanup(shell.gcl);
			free(shell.gcl);
			exit (shell.error_code);
		}
		printf("user_input : %s\n", shell.user_input);
	}
	shell.error_code = 666;
	return (shell.error_code);
}
