/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:15:58 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/27 21:11:04 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "ast.h"
# include "minishell.h"
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h>

# define COPY_MODE 0644

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}				t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	struct s_redir	*next;
}				t_redir;

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
	t_redir		*redir_list;
}				t_exec_context;

typedef struct s_pipe_exec_params
{
	t_pipe_node		*pipe_node;
	t_exec_context	*context;
	int				pipefd[2];
}				t_pipe_exec_params;

typedef struct s_external_exec_params
{
	t_cmd_node		*cmd_node;
	t_exec_context	*context;
	char			*cmd_path;
}				t_external_exec_params;

typedef struct s_subshell_exec_params
{
	t_subshell_node	*subshell_node;
	t_exec_context	*context;
}				t_subshell_exec_params;

typedef struct s_heredoc_params
{
	const char		*delimiter;
	t_exec_context	*context;
	int				pipefd[2];
}				t_heredoc_params;

typedef struct s_here_helper
{
	char	*line;
	char	*delim;
	char	*tmp;
	char	*varline;
	int		count_line;
	int		fd;
}				t_here_helper;

int		ms_execute_ast(t_ast_node *node, t_exec_context *context);
int		ms_execute_command(t_cmd_node *cmd_node, t_exec_context *context,
			t_gc *gcl);
int		ms_execute_external(t_cmd_node *cmd_node, t_exec_context *context,
			t_gc *gcl);

int		ms_execute_pipeline(t_ast_node *node, t_exec_context *context);

int		ms_execute_logical(t_ast_node *node, t_exec_context *context);
int		ms_execute_subshell(t_subshell_node *subshell_node, t_exec_context *context);

t_redir	*ms_collect_redirections(t_ast_node *node, t_gc *gcl, t_shell *shell);
int		ms_apply_redirections(t_redir *redir_list);

char	*ms_parse_cmd_path(const char *command, t_shell *shell);
char	*ms_concat_path(const char *path, const char *command, t_gc *gcl);

int		ms_handle_error(const char *msg, int exit_status, t_gc *gcl);

int		ms_heredoc_mode(t_heredoc_node *node, t_exec_context *context);
void	st_heredoc_child_process(t_shell *shell,
			char *delimiter, char *filename);

int		init_io(int new_in, int new_out, int new_err);
int		safe_pipe(int pipefd[2]);
int		safe_open(const char *filepath, int flags, int mode);
int		safe_close(int fd);
#endif /* EXEC_H */
