/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:48:33 by jeportie          #+#    #+#             */
/*   Updated: 2024/09/24 15:05:00 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_shell
{
	char	*user_input;
	int		error_code;
}			t_shell;

t_shell	ms_init_shell(int argc, char **envp); /*A tester*/
void	ms_get_user_input(t_shell *shell); /*A tester*/

#endif /*MINISHELL_H*/
