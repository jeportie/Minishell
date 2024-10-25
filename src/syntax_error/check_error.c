/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:12:29 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/24 16:34:10 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

static void	st_print_error_operator_redir(int error)
{
	if (error == 1)
		printf("Minishell: syntax error near unexpected token `newline'\n");
	else if (error == 2)
		printf("Minishell: syntax error near unexpected token `<'\n");
	else if (error == 3)
		printf("Minishell: syntax error near unexpected token `<<'\n");
	else if (error == 4)
		printf("Minishell: syntax error near unexpected token `>'\n");
	else if (error == 5)
		printf("Minishell: syntax error near unexpected token `>>'\n");
	else if (error == 6)
		printf("Minishell: syntax error near unexpected token `&'\n");
	else if (error == 7)
		printf("Minishell: syntax error near unexpected token `&&'\n");
	else if (error == 8)
		printf("Minishell: syntax error near unexpected token `||'\n");
	else if (error == 9)
		printf("Minishell: syntax error near unexpected token `|'\n");
}

int	check_error(t_syntax **syntax)
{
	t_syntax	*synt;

	synt = *syntax;
	if (synt->p_trigger > 0 || synt->sq_trigger || synt->dq_trigger
		|| synt->error > 0)
	{
		if (synt->error > 0)
			st_print_error_operator_redir(synt->error);
		else if (synt->sq_trigger == 1)
			printf("Minishell: syntax error near unexpected token `\''\n");
		else if (synt->dq_trigger == 1)
			printf("Minishell: syntax error near unexpected token `\"'\n");
		else if (synt->p_trigger == 1)
			printf("Minishell: syntax error near unexpected token `('\n");
		else if (synt->p_trigger == 2)
			printf("Minishell: syntax error near unexpected token `)'\n");
		synt->error = 1;
	}
	return (synt->error);
}
