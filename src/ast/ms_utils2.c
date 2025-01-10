/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:52:26 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 10:37:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

bool	ms_is_redir_op(t_token *current_token)
{
	return (current_token->type == TOKEN_REDIRECTION);
}

bool	ms_is_sbs_start(t_token *current_token)
{
	return (current_token->type == TOKEN_SUBSHELL_START);
}

bool	ms_is_sbs_stop(t_token *current_token)
{
	return (current_token->type == TOKEN_SUBSHELL_STOP);
}

void	ms_print_branch(int depth, int is_left, int fd)
{
	if (depth > 0)
	{
		if (is_left)
			ft_dprintf(fd, "├── ");
		else
			ft_dprintf(fd, "└── ");
	}
	else
		ft_dprintf(fd, "└── ");
}
