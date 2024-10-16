/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:48:33 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/15 13:08:14 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libgc/include/libgc.h"
# include "../lib/libft/include/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>

extern int	g_signal;

typedef struct s_env
{
	char			*var;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct s_env_data
{
	bool	env_exist;
	int		shell_lvl;
	t_env	*env;
	char	*path;
	char	*term;
	char	*shell;
}				t_env_data;

typedef struct s_shell
{
	t_env_data	*env_data;
	char		*user_input;
	int			error_code;
	t_gc		*gcl;
}				t_shell;

/* Init_shell */
t_shell		ms_init_shell(int argc, char **argv, char **envp);
void		ms_get_user_input(t_shell *shell);
t_env_data	*ms_init_env(char **envp, t_shell *shell);

/* env_lst_utils */
t_env		*ms_env_create_node(t_shell *shell, const char *env_line);
void		ms_env_add_back(t_env **env, t_env *new_node);

/* signal */
void		ms_init_std_signal(void);
void		ms_stop_std_signal(void);

#endif /*MINISHELL_H*/
