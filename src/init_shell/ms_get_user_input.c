/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_user_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:51:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/18 10:08:09 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_get_user_input(t_shell *shell)
{
	shell->user_input = readline("ptit'coque> ");
	if (g_signal)
	{
		shell->error_code = g_signal;
		g_signal = 0;
	}
	if (!shell->user_input)
		return ;
	add_history(shell->user_input);
}
