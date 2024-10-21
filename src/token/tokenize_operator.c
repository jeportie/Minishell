/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:52:27 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 12:53:29 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenize.h"

t_token	*tokenize_operator(const char **input, t_gc *gcl)
{
	const t_operator_map	operator_map[] = {
	{"||", TOKEN_OR},
	{"&&", TOKEN_AND},
	{"<<", TOKEN_REDIRECTION},
	{"<", TOKEN_REDIRECTION},
	{">>", TOKEN_REDIRECTION},
	{">", TOKEN_REDIRECTION},
	{"|", TOKEN_PIPE},
	{NULL, 0}};
	int						i;
	char					*current;
	t_token_type			type;
	char					*value;
	char					*op_str;
	int						op_len;

	if (!input || !*input)
	{
		gc_cleanup(gcl);
		perror("Minishell: Error: null input\n");
		exit(EXIT_FAILURE);
	}
	current = (char *)*input;
	i = 0;
	while (operator_map[i].op_str)
	{
		op_str = (char *)operator_map[i].op_str;
		op_len = ft_strlen(op_str);
		if (!ft_strncmp(current, op_str, op_len))
		{
			type = operator_map[i].type;
			value = (char *)gc_malloc(sizeof(char) * (op_len + 1), gcl);
			ft_strlcpy(value, op_str, op_len + 1);
			gc_lock(value, gcl);
			*input += op_len;
			return (create_token(type, value, gcl));
		}
		i++;
	}
	value = (char *)gc_malloc(sizeof(char) * 2, gcl);
	value[0] = *current;
	value[1] = '\0';
	gc_lock(value, gcl);
	*input += 1;
	return (create_token(TOKEN_WORD, value, gcl));
}
