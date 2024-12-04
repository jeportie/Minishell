/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 12:48:33 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/04 13:47:49 by jeportie         ###   ########.fr       */
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

# define DEBUG 0 
# define PROMPT_MODE 0
# define PRINT_INFOS "assets/infos.txt"
# define COPY_MODE 0644

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
	char	*user;
}				t_env_data;

typedef struct s_shell
{
	t_env_data	*env_data;
	char		*user_input;
	int			error_code;
	bool		interactive_mode;
	t_gc		*gcl;
}				t_shell;

/* Init_shell */
t_shell		ms_init_shell(int argc, char **argv, char **envp);
void		ms_get_user_input(t_shell *shell);
void		init_fix_value(t_env_data *env_data);
t_env_data	*ms_init_env(char **envp, t_shell *shell);

/* env_lst_utils */
t_env		*ms_env_create_node(t_shell *shell, const char *env_line);
void		ms_env_add_back(t_env **env, t_env *new_node);

/* signal */
int			rl_event_dummy(void);
void		ms_init_parent_cmd_signal(void);
void		ms_init_child_cmd_signal(void);
void		ms_init_std_signal(void);
void		ms_stop_std_signal(void);

char		*ms_get_env_value(t_env *env, char *name, int error_code);
void		ms_set_env_value(t_shell *shell, char *name, char *value);
char		**ms_get_envp(t_env *env, t_gc *gcl);
char		*ms_getenv(const char *name, t_env_data *env_data);

#endif /*MINISHELL_H*/
