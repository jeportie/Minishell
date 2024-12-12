/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:15:58 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/12 18:04:31 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "ast.h"
# include "minishell.h"
# include "process.h"
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h>

# define COPY_MODE 0644

typedef struct s_exec_context
{
	int			stdin_fd;
	int			stdout_fd;
	int			stderr_fd;
	t_shell		*shell;
	bool		is_subprocess;
	int			exit_status;
	int			child_lvl;
	bool		redirected;
	int			original_stdin;
	int			original_stdout;
	int			tmpfile_counter;
}				t_exec_context;

typedef struct s_pipe_exec_params
{
	t_pipe_node		*pipe_node;
	t_exec_context	*context;
	t_proc_manager	*manager;
	int				pipefd[2];
}				t_pipe_exec_params;

typedef struct s_external_exec_params
{
	t_cmd_node		*cmd_node;
	t_exec_context	*context;
	t_proc_manager	*manager;
	char			*cmd_path;
}				t_external_exec_params;

typedef struct s_subshell_exec_params
{
	t_subshell_node	*subshell_node;
	t_exec_context	*context;
	t_proc_manager	*manager;
}				t_subshell_exec_params;

typedef struct s_heredoc_params
{
	const char		*delimiter;
	t_exec_context	*context;
	t_proc_manager	*manager;
	int				pipefd[2];
}				t_heredoc_params;

int		ms_execute_ast(t_ast_node *node, t_exec_context *context,
			t_proc_manager *manager);

int		ms_execute_command(t_cmd_node *cmd_node, t_exec_context *context,
			t_proc_manager *manager, t_gc *gcl);
int		ms_execute_external(t_cmd_node *cmd_node, t_exec_context *context,
			t_proc_manager *manager, t_gc *gcl);

int		ms_execute_pipeline(t_pipe_node *pipe_node, t_exec_context *context,
			t_proc_manager *manager);

void	left_child_process(t_pipe_exec_params *params);
void	right_child_process(t_pipe_exec_params *params);
int		parent_process(pid_t left_pid, pid_t right_pid,
			t_exec_context *context);

int		ms_execute_logical(t_logic_node *logic_node, t_exec_context *context,
			t_node_type type, t_proc_manager *manager);
int		ms_execute_subshell(t_subshell_node *subshell_node,
			t_exec_context *context, t_proc_manager *manager);
int		ms_handle_redirections(t_ast_node *node, t_exec_context *context,
			t_proc_manager *manager, t_gc *gcl);

char	*ms_parse_cmd_path(const char *command, t_shell *shell);
char	*ms_concat_path(const char *path, const char *command, t_gc *gcl);

int		ms_handle_error(const char *msg, int exit_status, t_gc *gcl);
void	fork_init(t_fork_params *fork_params, t_exec_context *context,
			bool is_here_doc, char *title);

int		ms_heredoc_mode(const char *delimiter, t_exec_context *context,
			t_proc_manager *manager, t_gc *gcl);
void	ft_generate_temp_filename(char *buffer, size_t buffer_size,
			t_exec_context *context);
#endif /* EXEC_H */
