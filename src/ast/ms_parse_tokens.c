/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 08:31:06 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/24 14:24:26 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../include/minishell.h"
#include "../../include/ast.h"
#include "../../include/tokenize.h"

t_ast_node	*create_logical_node(t_node_type type, t_ast_node *left,
	t_ast_node *right, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = type;
	node->data.logic.left = left;
	node->data.logic.right = right;
	return (node);
}

t_ast_node	*create_pipe_node(t_ast_node *left, t_ast_node *right, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = NODE_PIPE;
	node->data.pipe.left = left;
	node->data.pipe.right = right;
	return (node);
}

t_ast_node	*create_redirect_node(t_node_type type, t_ast_node *child, char *filename, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = type;
	node->data.redirect.type = type;
	node->data.redirect.child = child;
	node->data.redirect.filename = filename;
	return (node);
}

t_ast_node	*create_command_node(t_token **current_token, t_gc *gcl)
{
	t_cmd_node	command_data;
	t_ast_node	*command_node;
	int			argc;
	char		**argv;
	size_t		argv_size;
	t_token		*token;

	argc = 0;
	argv_size = 4;
	argv = (char **)gc_malloc(sizeof(char *) * argv_size, gcl);
	gc_lock(argv, gcl);
	while (*current_token && (*current_token)->type == TOKEN_WORD)
	{
		if (argc >= argv_size)
		{
			argv_size *= 2;
			/*NOTE: gc_realloc needs to be implemented*/
			argv = gc_realloc(argv, sizeof(char *) * argv_size, gcl);
			gc_lock(argv, gcl);
		}
		argv[argc] = (*current_token)->token;
		argc++;
		*current_token = (*current_token)->next;
	}
	argv[argc] = NULL;
	command_data.argv = argv;
	command_data.argc = argc;
	command_node = gc_malloc(sizeof(t_ast_node), gcl);
	if (!command_node)
		return (NULL);
	command_node->type = NODE_COMMAND;
	command_node->data.command = command_data;
	return (command_node);
}

t_ast_node	*create_heredoc_node(t_ast_node *child, char *delimiter, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = NODE_REDIRECT_HEREDOC;
	node->data.heredoc.child = child;
	node->data.heredoc.delimiter = delimiter;
	return (node);
}

t_ast_node	*create_subshell_node(t_ast_node *child, t_gc *gcl)
{
	t_ast_node	*node;

	node = (t_ast_node *)gc_malloc(sizeof(t_ast_node), gcl);
	gc_lock(node, gcl);
	node->type = NODE_SUBSHELL;
	node->data.heredoc.child = child;
	return (node);
}

t_ast_node	*parse_subshell(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*subshell_content;
	t_ast_node	*node;

	if (!*current_token || (*current_token)->type != TOKEN_SUBSHELL_START)
	{
        fprintf(stderr, "Syntax Error: Expected '('.\n");
        return NULL;
	}
	*current_token = (*current_token)->next;
	subshell_content = parse_logical(current_token, gcl);
	if (!subshell_content)
		return (NULL);
	if (!*current_token || (*current_token)->type != TOKEN_SUBSHELL_STOP)
	{
        fprintf(stderr, "Syntax Error: Expected ')'.\n");
        return NULL;
	}
	*current_token = (*current_token)->next;
	node = create_subshell_node(subshell_content, gcl);
	return (subshell_content);
}

t_ast_node	*parse_redirection(t_token **current_token, t_ast_node *child, t_gc *gcl)
{
	t_node_type	redir_type;
	char		*filename;
	t_token		*token;

	if ((*current_token)->type != TOKEN_REDIRECTION)
		return (child);
	if (ft_strncmp((*current_token)->token, "<", 1) == 0)
		redir_type = NODE_REDIRECT_IN;
	else if (ft_strncmp((*current_token)->token, ">", 1) == 0)
		redir_type = NODE_REDIRECT_OUT;
	else if (ft_strncmp((*current_token)->token, ">>", 1) == 0)
		redir_type = NODE_REDIRECT_APPEND;
	else if (ft_strncmp((*current_token)->token, "<<", 1) == 0)
		redir_type = NODE_REDIRECT_HEREDOC;
	else
	{
		fprintf(stderr, "Syntax Error: Unknown redirection '%s'.\n", (*current_token)->token);
		return (NULL);
	}
	filename = (*current_token)->token;
	*current_token = (*current_token)->next;
	if (redir_type == NODE_REDIRECT_HEREDOC)
		child = create_heredoc_node(child, filename, gcl);
	else
		child = create_redirect_node(redir_type, child, filename, gcl);
	while(*current_token && (*current_token)->type == TOKEN_REDIRECTION)
	{
		child = parse_redirection(current_token, child, gcl);
		if (!child)
			return (NULL);
	}
	return (child);
}

t_ast_node	*parse_command(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*node;

	if (*current_token && (*current_token)->type == TOKEN_SUBSHELL_START)
	{
		node = parse_subshell(current_token, gcl);
		if (!node)
			return (NULL);
		while (*current_token && (*current_token)->type == TOKEN_REDIRECTION)
		{
			node = parse_redirection(current_token, node, gcl);
			if (!node)
				return (NULL);
		}
		return (node);
	}
	if (!*current_token || (*current_token)->type != TOKEN_WORD)
	{
		perror("Minishell: Syntax Error: Expected a command.\n");
		return (NULL);
	}
	node = create_command_node(current_token, gcl);
	if (!node)
		return (NULL);
	while (*current_token && (*current_token)->type == TOKEN_REDIRECTION)
	{
		node = parse_redirection(current_token, node, gcl);
		if (!node)
			return (NULL);
	}
	return (node);
}

bool	is_pipe_op(t_token *current_token)
{
	return (current_token->type == TOKEN_PIPE);
}

t_ast_node	*parse_pipeline(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*left_node;
	t_ast_node	*right_node;

	left_node = parse_command(current_token, gcl);
	if (!left_node)
		return (NULL);
	while (*current_token && is_pipe_op(*current_token))
	{
		*current_token = (*current_token)->next;
		right_node = parse_command(current_token, gcl);
		if (!right_node)
		{
			perror("Minishell: Syntax Error: Expected Command after pipe '|'.\n");
			return (NULL);
		}
		left_node = create_pipe_node(left_node, right_node, gcl);
	}
	return (left_node);
}

bool	is_logical_op(t_token *current_token)
{
	if (current_token->type == TOKEN_AND || current_token->type == TOKEN_OR)
		return (true);
	return (false);
}

t_ast_node	*parse_logical(t_token **current_token, t_gc *gcl)
{
	t_ast_node	*left_node;
	t_ast_node	*right_node;
	t_node_type	op_type;

	left_node = parse_pipeline(current_token, gcl);
	if (!left_node)
		return (NULL);
	while (*current_token && is_logical_op(*current_token))
	{
		if ((*current_token)->type == TOKEN_AND)
			op_type = NODE_AND;
		else if ((*current_token)->type == TOKEN_OR)
			op_type = NODE_OR;
		else
			return (left_node);
		*current_token = (*current_token)->next;
		right_node = parse_pipeline(current_token, gcl);
		if (!right_node)
		{
			perror("Minishell: Syntax Error: Expected Command after logical operator.\n");
			return (NULL);
		}
		left_node = create_logical_node(op_type, left_node, right_node, gcl);
	}
	return (left_node);
}

t_ast_node	*ms_parse_tokens(t_token *tokens, t_gc *gcl)
{
	t_ast_node	*ast_root;
	t_token		*current_token;

	if (tokens == NULL)
	{
		perror("Minishell: Error: No Input provided.\n");
		gc_cleanup(gcl);
		exit(EXIT_FAILURE);
	}
	current_token = tokens;
	ast_root = parse_logical(&current_token, gcl);
	if (current_token != NULL)
	{
		fprintf(stderr, "Minishell: Syntax Error: Unexpected token '%s'\n", current_token->token);
		gc_cleanup(gcl);
		exit (EXIT_FAILURE);
	}
	if (!ast_root)
	{
		perror("Minishell: Error: Failed to parse tokens.\n");
		gc_cleanup(gcl);
		exit(EXIT_FAILURE);
	}
	return (ast_root);
}
