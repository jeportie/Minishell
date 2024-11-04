/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:15:58 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 12:45:39 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# define COPY_MODE 0644

# include "ast.h"
# include "minishell.h"
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

typedef struct s_exec_context
{
	int			stdin_fd;
	int			stdout_fd;
	int			stderr_fd;
	t_shell		*shell;
	bool		is_subprocess;
	int			exit_status;
}				t_exec_context;

typedef struct s_pipe_context
{
	t_exec_context	*context;
	int				pipefd[2];
	pid_t			last_pid;
	int				last_status;
}				t_pipe_context;

typedef struct s_process
{
	pid_t		pid;
	bool		is_last;
}				t_process;

typedef struct s_pipeline
{
	t_process	*processes;
	int			num_processes;
}				t_pipeline;

int		ms_execute_ast(t_ast_node *node, t_exec_context *context);

int		ms_execute_command(t_cmd_node *cmd_node, t_exec_context *context, t_gc *gcl);
int		ms_execute_external(t_cmd_node *cmd_node, t_exec_context *context, t_gc *gcl);

int		ms_execute_pipeline(t_pipe_node *pipe_node, t_exec_context *context);
int		ms_execute_logical(t_logic_node *logic_node, t_exec_context *context, t_node_type type);
int		ms_execute_subshell(t_subshell_node *subshell_node, t_exec_context *context);
int		ms_handle_redirections(t_ast_node *node, t_exec_context *context);

char	**ms_get_envp(t_env *env, t_gc *gcl);
char	*ms_parse_cmd_path(const char *command, t_shell *shell);
char	*ms_concat_path(const char *path, const char *command, t_gc *gcl);
char	*ms_getenv(const char *name, t_env_data *env_data);

int		ms_handle_error(const char *msg, t_gc *gcl); //returns 1
int		ms_heredoc_mode(const char *delimiter, t_exec_context *context, t_gc *gcl);
char	**ms_expand_wild(const char *pattern, t_gc *gcl);
char	**ms_expand_var(const char *pattern, t_gc *gcl);

#endif /* EXEC_H */
