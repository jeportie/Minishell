/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_child_cmd_signal.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:55:03 by gmarquis          #+#    #+#             */
/*   Updated: 2024/11/22 16:50:16 by gmarquis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	st_sigint_cmd_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		exit(130);
	}
}

static void	st_sigquit_cmd_handler(int sig)
{
	if (sig == SIGQUIT)
	{
		g_signal = 131;
		exit(131);
	}
}

void	ms_init_child_cmd_signal(void)
{
	signal(SIGINT, st_sigint_cmd_handler);
	signal(SIGQUIT, st_sigquit_cmd_handler);
}
