/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_user_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:51:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/17 13:42:19 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>

char	*st_make_input(t_shell *shell)
{
	char	*shell_prompt;

	shell_prompt = gc_strjoin(shell->env_data->user, "@minishell $> ",
			shell->gcl);
	if (!shell_prompt)
		return ("minishell> ");
	return (shell_prompt);
}

/*
char	*st_make_input(t_shell *shell)
{
	char	*shell_prompt;
	char	*error_code;
	char	*cwd;

	error_code = ft_itoa(shell->error_code);
	gc_register(error_code, shell->gcl);
	cwd = getcwd(NULL, 0);
	gc_register(cwd, shell->gcl);
	shell_prompt = gc_strjoin("(", error_code, shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, ") ", shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, shell->env_data->user, shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, "@minishell", shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, " | ", shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, cwd, shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, " $> ", shell->gcl);
	if (!shell_prompt)
		return ("minishell> ");
	return(shell_prompt);
}
*/

void	ms_get_user_input(t_shell *shell)
{
	rl_event_hook = rl_event_dummy;
	if (shell->interactive_mode)
		shell->user_input = readline(st_make_input(shell));
	else
	{
		shell->user_input = get_next_line(STDIN_FILENO);
		gc_register(shell->user_input, shell->gcl);
		gc_lock(shell->user_input, shell->gcl);
		if (shell->user_input)
		{
			size_t (len) = ft_strlen(shell->user_input);
			if (len > 0 && shell->user_input[len - 1] == '\n')
				shell->user_input[len - 1] = '\0';
		}
	}
	if (g_signal == 132)
	{
		shell->error_code = 130;
		return (g_signal = 0, ms_get_user_input(shell));
	}
	if (!shell->user_input)
		return ;
	if (shell->user_input[0] == '\0')
		ms_get_user_input(shell);
	add_history(shell->user_input);
}
