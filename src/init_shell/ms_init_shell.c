/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:40:35 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/02 11:53:37 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_shell	ms_init_shell(int argc, char **argv, char **envp)
{
	t_shell	shell;

	if (!argc || !argv)
		exit(EXIT_FAILURE);
	if (argc > 1 || argv[1])
	{
		perror("Minishell: format: no arguments needed!\n");
		exit(EXIT_FAILURE);
	}
	ft_memset(&shell, 0, sizeof(t_shell));
	shell.gcl = malloc(sizeof(t_gc));
	gc_register(shell.gcl, *shell.gcl);
	gc_lock(shell.gcl, *shell.gcl);
	//shell.gcl->head = NULL;
	//shell.user_input = NULL;
	//shell.error_code = 0;
	shell.env_data = ms_init_env(envp, &shell);
	return (shell);
}
