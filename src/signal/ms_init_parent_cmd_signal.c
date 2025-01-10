/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init_parent_cmd_signal.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 11:55:03 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 15:36:38 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	st_sigint_cmd_handler(int sig)
{
	if (sig == SIGINT)
		ft_printf("\n");
	g_signal = 130;
}

static void	st_sigquit_cmd_handler(int sig)
{
	if (sig == SIGQUIT)
		ft_printf("Quitter (core dumped).\n");
	g_signal = 131;
}

void	ms_init_parent_cmd_signal(void)
{
	signal(SIGINT, st_sigint_cmd_handler);
	signal(SIGQUIT, st_sigquit_cmd_handler);
}
