/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:33:09 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/31 14:18:07 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
# define SYNTAX_H

# include "minishell.h"

typedef struct s_syntax
{
	char	*current;
	int		error;
	int		i;
	int		len_end;
	int		sq_trigger;
	int		dq_trigger;
	int		p_trigger;
	int		p_char;
	int		o_and;
	int		o_or;
	int		o_pipe;
	int		r_in;
	int		r_out;
	int		r_app;
	int		r_here;
}	t_syntax;

/* Main fonction */
int	ms_syntax_error(const char *input);

int	check_error(t_syntax **syntax);
int	check_reinit(t_syntax **syntax);
int	operator_gestion(t_syntax **syntax);
int	parenthesis_gestion(t_syntax **syntax);
int	quote_gestion(t_syntax **syntax);
int	redir_gestion(t_syntax **syntax);

#endif /*SYNTAX_H*/
