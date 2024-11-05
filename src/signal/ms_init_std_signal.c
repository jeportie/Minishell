/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_std_signal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:55:03 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/05 15:54:31 by jeportie         ###   ########.fr       */
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

//void	ms_init_std_signal(void)
//{
//	signal(SIGQUIT, SIG_IGN);
//	signal(SIGINT, st_sigint_std_handler);
//}

void	ms_init_std_signal(void)
{
	struct sigaction sa;

	sa.sa_handler = st_sigint_std_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);
}
