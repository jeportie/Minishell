/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_signal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 14:22:00 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 13:27:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		rl_done = 1;
	}
}

void	ms_parent_here_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		rl_done = 1;
	}
}
