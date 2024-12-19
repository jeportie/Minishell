/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:40:35 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/19 17:07:16 by jeportie         ###   ########.fr       */
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
	shell.gcl = gc_init();
	shell.env_data = ms_init_env(envp, &shell);
	ft_bzero(&shell.heredocs, 16);
	return (shell);
}
