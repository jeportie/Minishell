/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_user_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:51:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/27 10:48:46 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>

void	ms_get_user_input(t_shell *shell)
{
	char	*shell_prompt;
	char	*error_code;
	char	*user;
	char	*cwd;

	error_code = ft_itoa(shell->error_code);
	gc_register(error_code, shell->gcl);
	cwd = getcwd(NULL, 0);
	gc_register(cwd, shell->gcl);
	user = ms_get_env_value(shell->env_data->env, "USER", shell->error_code);
	shell_prompt = gc_strjoin("(", error_code, shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, ") ", shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, user, shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, "@minishell", shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, " | ", shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, cwd, shell->gcl);
	shell_prompt = gc_strjoin(shell_prompt, " $> ", shell->gcl);
	rl_event_hook = rl_event_dummy;
	shell->user_input = readline(shell_prompt);
	if (g_signal == 132)
	{
		shell->error_code = 130;
		g_signal = 0;
		printf("ici\n");
		return (ms_get_user_input(shell));
	}
	gc_collect(shell->gcl);
	if (!shell->user_input)
		return ;
	add_history(shell->user_input);
}
