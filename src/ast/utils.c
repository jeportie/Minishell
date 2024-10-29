/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:01:16 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/29 13:02:42 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

bool	is_pipe_op(t_token *current_token)
{
	return (current_token->type == TOKEN_PIPE);
}

bool	is_logical_op(t_token *current_token)
{
	if (current_token->type == TOKEN_AND || current_token->type == TOKEN_OR)
		return (true);
	return (false);
}

bool	is_command_op(t_token *current_token)
{
	if (current_token->type == TOKEN_WORD
		|| current_token->type == TOKEN_EXPAND)
		return (true);
	return (false);
}

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
