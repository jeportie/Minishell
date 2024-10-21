/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:33:09 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 12:33:54 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_H
# define SYNTAX_H

# include "minishell.h"

/* Main fonction */
int	ms_syntax_error(const char *input, t_shell *shell);
int	ms_quote_error(const char *input, t_shell *shell);
int	ms_pipe_error(const char *input, t_shell *shell);
int	ms_operator_error(const char *input, t_shell *shell);

#endif /*SYNTAX_H*/

/*
##	Examinate and search

	Examinate a command line and search for syntaxe error beaford tokenization,
which make possible to avoid unesse unnecessary processes, like tokenization,
ast building and execution.

[]	Command error's

[]	Quote's error's
#[]		''Single quote's, for literal strings
##[]		Unmatched single quote:		cmd1 'word
#[]		""Double quote's, prevent from splitting
##[]		Unmatched double quote:		cmd1 "word

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
##[]		Unmatched parentheses:		(cmd1
##[]		Empty parentheses:		()

*/
