/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_std_signal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:55:03 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/15 12:49:21 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	st_sigint_std_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_signal = 130;
}

void	ms_init_std_signal(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, st_sigint_std_handler);
}
