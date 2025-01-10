/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:52:27 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 15:29:38 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

static const t_operator_map	*get_operator_map(void)
{
	static const t_operator_map	operator_map[] = {
	{"||", TOKEN_OR},
	{"&&", TOKEN_AND},
	{"<<", TOKEN_REDIRECTION},
	{"<", TOKEN_REDIRECTION},
	{">>", TOKEN_REDIRECTION},
	{">", TOKEN_REDIRECTION},
	{"|", TOKEN_PIPE},
	{NULL, 0}
	};

	return (operator_map);
}

static void	init_op_state(const char **input, t_op_state *op_state)
{
	const t_operator_map	*operator_map;

	ft_memset(op_state, 0, sizeof(t_op_state));
	operator_map = get_operator_map();
	op_state->op_map = operator_map;
	op_state->current = (char *)*input;
}

static t_token	*allocator(const char **input, t_op_state *v, t_gc *gcl)
{
	t_token_type	type;
	char			*value;

	type = v->op_map[v->i].type;
	value = (char *)gc_malloc(sizeof(char) * (v->op_len + 1), gcl);
	ft_strlcpy(value, v->op_str, v->op_len + 1);
	*input += v->op_len;
	return (create_token(type, value, gcl));
}

t_token	*tokenize_operator(const char **input, t_gc *gcl)
{
	t_op_state	v;

	init_op_state(input, &v);
	if (!input || !*input)
	{
		gc_cleanup(gcl);
		ft_dprintf(STDERR_FILENO, "Minishell: Error: null input\n");
		exit(EXIT_FAILURE);
	}
	v.i = 0;
	while (v.op_map[v.i].op_str)
	{
		v.op_str = (char *)v.op_map[v.i].op_str;
		v.op_len = ft_strlen(v.op_str);
		if (!ft_strncmp(v.current, v.op_str, v.op_len))
			return (allocator(input, &v, gcl));
		v.i++;
	}
	v.value = (char *)gc_malloc(sizeof(char) * 2, gcl);
	v.value[0] = *v.current;
	v.value[1] = '\0';
	*input += 1;
	return (create_token(TOKEN_WORD, v.value, gcl));
}
