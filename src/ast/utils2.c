/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:52:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/29 15:52:50 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

bool	is_redir_op(t_token *current_token)
{
	return (current_token->type == TOKEN_REDIRECTION);
}

bool	is_sbs_start(t_token *current_token)
{
	return (current_token->type == TOKEN_SUBSHELL_START);
}

bool	is_sbs_stop(t_token *current_token)
{
	return (current_token->type == TOKEN_SUBSHELL_STOP);
}
