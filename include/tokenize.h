/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:01:00 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/20 15:00:49 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_REDIRECTION,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_EXEC,
	TOKEN_SUBSHELL_START,
	TOKEN_SUBSHELL_STOP,
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*token;
	struct s_token	*next;
}			t_token;

/* Main Function */
t_token	*ms_tokenize(const char *commant_line, t_gc *gcl);

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
t_token	*tokenize_quote(const char **input, t_gc *gcl);
t_token	*tokenize_frame(const char **input, t_gc *gcl);
t_token	*tokenize_operator(const char **input, t_gc *gcl);
t_token	*map_token_type_to_operator(const char *operator_token);

#endif /*TOKENIZE_H*/
