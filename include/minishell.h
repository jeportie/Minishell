/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:48:33 by jeportie          #+#    #+#             */
/*   Updated: 2024/09/30 16:07:14 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libgc/include/libgc.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_shell
{
	char	*user_input;
	int		error_code;
	t_gc 	*gcl;
}			t_shell;

t_shell	ms_init_shell(int argc, char **argv, char **envp); /*A tester*/
void	ms_get_user_input(t_shell *shell); /*A tester*/

#endif /*MINISHELL_H*/
