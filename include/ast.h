/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:45:53 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/02 16:27:43 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "minishell.h"
# include "tokenize.h"

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_REDIRECT_APPEND,
	NODE_REDIRECT_HEREDOC,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL,
	ERR
}			t_node_type;

typedef struct s_cmd_node
{
	char	**argv;
	int		argc;
	bool	is_expand;
}				t_cmd_node;

typedef struct s_pipe_node
{
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}				t_pipe_node;

typedef struct s_redirect_node
{
	t_node_type			type;
	struct s_ast_node	*child;
	char				*filename;
}				t_redirect_node;

typedef struct s_heredoc_node
{
	t_node_type				type;
	struct s_ast_node		*child;
	char					*delimiter;
	char					*filename;
}				t_heredoc_node;

typedef struct s_logic_node
{
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}				t_logic_node;

typedef struct s_subshell_node
{
	struct s_ast_node	*child;
}				t_subshell_node;

typedef union u_node_data
{
	t_cmd_node			command;
	t_pipe_node			pipe;
	t_redirect_node		redirect;
	t_heredoc_node		heredoc;
	t_logic_node		logic;
	t_subshell_node		subshell;
}						t_node_data;

typedef struct s_ast_node
{
	t_node_type	type;
	t_node_data	data;
}				t_ast_node;

/* Main Function */
t_ast_node	*ms_parse_tokens(t_token *tokens, t_shell *shell, t_gc *gcl);

/* Parsing functions */
t_ast_node	*parse_sequence(t_token **current_token, t_gc *gcl);
t_ast_node	*parse_logical(t_token **current_token, t_shell *shell, t_gc *gcl);
t_ast_node	*parse_pipeline(t_token **current_token, t_shell *shell, t_gc *gcl);
t_ast_node	*parse_command(t_token **current_token, t_shell *shell, t_gc *gcl);
t_ast_node	*parse_redirection(t_token **current_token,
				t_ast_node *child, t_shell *shell, t_gc *gcl);
t_ast_node	*parse_subshell(t_token **current_token, t_shell *shell, t_gc *gcl);

/* Node Functions*/
t_ast_node	*create_command_node(t_token **current_token, t_gc *gcl);
t_ast_node	*create_pipe_node(t_ast_node *left, t_ast_node *right, t_gc *gcl);
t_ast_node	*create_redirect_node(t_node_type type, t_ast_node *child,
				char *filename, t_gc *gcl);
t_ast_node	*create_heredoc_node(t_node_type type, t_ast_node *child,
				char *delimiter, t_gc *gcl);
t_ast_node	*create_logical_node(t_node_type type, t_ast_node *left,
				t_ast_node *right, t_gc *gcl);
t_ast_node	*create_subshell_node(t_ast_node *child, t_gc *gcl);

/* Utils Functions */
bool		is_pipe_op(t_token *current_token);
bool		is_logical_op(t_token *current_token);
bool		is_command_op(t_token *current_token);
bool		is_redir_op(t_token *current_token);
bool		is_sbs_start(t_token *current_token);
bool		is_sbs_stop(t_token *current_token);
void		print_ast(t_ast_node *node, int depth, char *prefix, int is_left);
void		print_branch(int depth, int is_left, int fd);
void		print_ast_delimit(t_ast_node *root);

#endif /*AST_H*/
