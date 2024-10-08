/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:45:53 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/08 18:58:32 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

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
	NODE_SEQUENCE,
	NODE_BACKGROUND,
	NODE_SUBSHELL
}			t_node_type;

typedef struct s_cmd_node
{
	char	**argv;
	int		argc;
}				t_cmd_node;



#endif /*AST_H*/
