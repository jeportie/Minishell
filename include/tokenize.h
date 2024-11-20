/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:01:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/14 15:31:30 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# define STDERR 2

# include "minishell.h"

/*
 * NOTE:
 * TOKEN_EXPAND includes expand like wildcards or dollar, execs and commands
 */

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_EXPAND,
	TOKEN_REDIRECTION,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SUBSHELL_START,
	TOKEN_SUBSHELL_STOP,
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*token;
	struct s_token	*next;
}			t_token;

typedef struct s_operator_map
{
	const char		*op_str;
	t_token_type	type;
}					t_operator_map;

typedef struct s_op_state
{
	const t_operator_map	*op_map;
	int						i;
	char					*current;
	t_token_type			type;
	char					*value;
	char					*op_str;
	int						op_len;
}				t_op_state;

typedef struct s_token_state
{
	const char	*current;
	int			i;
	int			len;
	char		quote;
	bool		in_quote;
	bool		is_expand;
	char		start_quote;
	bool		stop;
}				t_token_state;

typedef struct s_copy_state
{
	const char	*current;
	char		*value;
	int			i;
	int			j;
	char		quote;
	bool		in_quote;
	bool		stop;
}				t_copy_state;

/* Main Function */
t_token	*ms_tokenize(const char *commant_line, t_gc *gcl);
int		token_len(const char *current, char start_quote, bool *is_expand);

/* Token List Functions */
t_token	*create_token(t_token_type type, const char *value, t_gc *gcl);
void	add_token(t_token **head, t_token *new_token, t_gc *gcl);

/* Utils functions*/
bool	is_whitespace(char c);
bool	is_operator(char c);
bool	is_quote(char c);
bool	is_frame(char c);
void	skip_whitespace(const char **input);
void	print_token(t_token *head);

/* Tokenize Functions */
t_token	*tokenize_word(const char **input, t_gc *gcl);
t_token	*tokenize_operator(const char **input, t_gc *gcl);
t_token	*tokenize_quote(const char **input, t_gc *gcl);
t_token	*tokenize_frame(const char **input, t_gc *gcl);

#endif /*TOKENIZE_H*/
