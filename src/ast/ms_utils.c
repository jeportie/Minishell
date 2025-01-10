/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:01:16 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 10:37:02 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/tokenize.h"

bool	ms_is_pipe_op(t_token *current_token)
{
	return (current_token->type == TOKEN_PIPE);
}

bool	ms_is_logical_op(t_token *current_token)
{
	if (current_token->type == TOKEN_AND || current_token->type == TOKEN_OR)
		return (true);
	return (false);
}

bool	ms_is_command_op(t_token *current_token)
{
	if (current_token->type == TOKEN_WORD
		|| current_token->type == TOKEN_EXPAND)
		return (true);
	return (false);
}
