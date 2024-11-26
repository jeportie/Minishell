/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_std_signal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:55:03 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/26 15:21:10 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	st_sigint_std_handler(int sig)
{
	rl_done = 1;
	if (sig == SIGINT)
		g_signal = 130;
	//{
	//	printf("\n");
	//	rl_on_new_line();
	//	rl_replace_line("", 0);
	//	rl_redisplay();
	//}
	//g_signal = 130;
}

int rl_event_dummy() { return 0; }

void	ms_init_std_signal(void)
{
	signal(SIGINT, st_sigint_std_handler);
	signal(SIGQUIT, SIG_IGN);
}
