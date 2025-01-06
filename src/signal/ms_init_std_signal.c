/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_std_signal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:55:03 by gmarquis          #+#    #+#             */
/*   Updated: 2024/11/27 10:51:15 by gmarquis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	st_sigint_std_handler(int sig)
{
	rl_done = 1;
	if (sig == SIGINT)
		g_signal = 132;
}

void	ms_init_std_signal(void)
{
	signal(SIGINT, st_sigint_std_handler);
	signal(SIGQUIT, SIG_IGN);
}
