/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:40:35 by jeportie          #+#    #+#             */
/*   Updated: 2024/09/30 16:08:58 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_shell	ms_init_shell(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	(void)envp;
	shell.user_input = NULL;
	shell.error_code = 0;
	shell.gcl = malloc(sizeof(t_gc));
	shell.gcl->head = NULL;
	gc_register(shell.gcl, *shell.gcl);
	gc_lock(shell.gcl, *shell.gcl);
	return (shell);
}
