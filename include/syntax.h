/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:33:09 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 09:21:49 by jeportie         ###   ########.fr       */
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
	int		p_open;
	int		p_close;
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

int	ms_find_close_par(char *current, int i);
int	ms_check_error(t_syntax **syntax);
int	ms_check_reinit(t_syntax **syntax);
int	ms_operator_gestion(t_syntax **syntax);
int	ms_parenthesis_gestion(t_syntax **syntax);
int	ms_quote_gestion(t_syntax **syntax);
int	ms_redir_gestion(t_syntax **syntax);

#endif /*SYNTAX_H*/
