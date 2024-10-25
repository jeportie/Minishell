/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:33:09 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/24 16:30:52 by jeportie         ###   ########.fr       */
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

/*
##	Examinate and search

	Examinate a command line and search for syntaxe error beaford tokenization,
which make possible to avoid unesse unnecessary processes, like tokenization,
ast building and execution.

[]	Command error's

[o]	Quote's error's
#[o]		''Single quote's, for literal strings
##[o]		Unmatched single quote:		cmd1 'word
#[o]		""Double quote's, prevent from splitting
##[o]		Unmatched double quote:		cmd1 "word

[]	Pipe's error's
#[]		Empty pipe:		cmd1 | | cmd2
#[]		Pipe at start or end of line:		| cmd1 or cmd1 |

[]	Redirection's error's
#[]		Ambiguous redirection's:		cmd1 <> or cmd1 ><
#[]		Redirection without file:		cmd1 >
#[]		To many heredocs (>16):		<< EOF << EOF << EOF ...

[]	Logical operator's error's
#[]		&& || logical operator
##[]		Empty operator:		cmd1 && && cmd2
##[]		Logical operator at start or end of line:		|| cmd1 or cmd1 ||
#[]		()Parenthese's
##[o]		Unmatched parentheses:		(cmd1
##[]		Empty parentheses:		()

*/
