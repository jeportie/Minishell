/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_redirections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:12:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/12 18:24:48 by jeportie         ###   ########.fr       */
/*   Updated: 2024/12/10 13:39:55 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/expand.h"
#include <fcntl.h>

int	safe_open2(const char *filepath, int flags, int mode, t_shell *shell)
{
	int		fd;

	if (is_var((char *)filepath))
	{
		filepath = nested_vars((char *)filepath, shell);
	}
	fd = open(filepath, flags, mode);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "Open failed\n");
		return (-1);
	}
	return (fd);
}

/*
static int	redirect_helper(t_ast_node *node, t_node_type type,
		t_exec_context *context, t_redirect_node *redir_node)
{
	int	fd;

	mode_t (mode) = 0;
	if (node->type == type)
	{
		redir_node = &node->data.redirect;
		fd = safe_open2(redir_node->filename, O_RDONLY, mode, context->shell);
		if (fd == -1)
			return (-1);
		if (context->stdin_fd != STDIN_FILENO)
			close(context->stdin_fd);
		context->stdin_fd = fd;
	}
	redir_node = &node->data.redirect;
	if (flags != O_RDONLY)
		mode = COPY_MODE;
	fd = ms_open_file(redir_node->filename, flags, mode);
	if (fd == -1)
		return (-1);
	if (*context_fd != std_fd)
		close(*context_fd);
	*context_fd = fd;
	return (0);
}
*/

static int	redirect_mode(t_ast_node *node, t_exec_context *context,
		t_proc_manager *manager, t_gc *gcl)
{
	t_redirect_node	*redir_node;
	t_heredoc_node	*heredoc_node;
	int				fd;
	mode_t			mode;

	mode = 0;
	if (node->type == NODE_REDIRECT_IN)
	{
		redir_node = &node->data.redirect;
		fd = safe_open2(redir_node->filename, O_RDONLY, mode, context->shell);
		if (fd == -1)
			return (-1);
		if (context->stdin_fd != STDIN_FILENO)
			close(context->stdin_fd);
		context->stdin_fd = fd;
	}
	else if (node->type == NODE_REDIRECT_OUT)
	{
		redir_node = &node->data.redirect;
		mode = COPY_MODE;
		fd = safe_open2(redir_node->filename, O_WRONLY | O_CREAT
				| O_TRUNC, mode, context->shell);
		if (fd == -1)
			return (-1);
		if (context->stdout_fd != STDOUT_FILENO)
			close(context->stdout_fd);
		context->stdout_fd = fd;
	}
	else if (node->type == NODE_REDIRECT_APPEND)
	{
		redir_node = &node->data.redirect;
		mode = COPY_MODE;
		fd = safe_open2(redir_node->filename, O_WRONLY | O_CREAT
				| O_APPEND, mode, context->shell);
		if (fd == -1)
			return (-1);
		if (context->stdout_fd != STDOUT_FILENO)
			close(context->stdout_fd);
		context->stdout_fd = fd;
	}
	else if (node->type == NODE_REDIRECT_HEREDOC)
	{
		heredoc_node = &node->data.heredoc;
		if (ms_heredoc_mode(heredoc_node->delimiter,
				context, manager, gcl) != 0)
			return (ms_handle_error("Minishell: Error: heredoc failed.\n",
					-1, gcl));
	}
	else
		return (ms_handle_error("Minishell: Error: "
				"unsupported redirection type.\n", -1, gcl));
	return (0);
}

t_ast_node	**redirection_nodes(t_ast_node *node, t_gc *gcl)
{
	t_ast_node	**child_array;
	t_ast_node	**result;
	size_t		len;
	size_t		i;
	t_ast_node	*child;

	if (!node)
	{
		result = gc_malloc(sizeof(t_ast_node *), gcl);
		result[0] = NULL;
		return (result);
	}
	if (node->type == NODE_REDIRECT_IN || node->type == NODE_REDIRECT_OUT
		|| node->type == NODE_REDIRECT_APPEND
		|| node->type == NODE_REDIRECT_HEREDOC)
	{
		if (node->type == NODE_REDIRECT_HEREDOC)
			child = node->data.heredoc.child;
		else
			child = node->data.redirect.child;
		child_array = redirection_nodes(child, gcl);
		len = 0;
		while (child_array[len])
			len++;
		result = gc_malloc(sizeof(t_ast_node *) * (len + 2), gcl);
		i = 0;
		while (i < len)
		{
			result[i] = child_array[i];
			i++;
		}
		result[len] = node;
		result[len + 1] = NULL;
		return (result);
	}
	else
	{
		result = gc_malloc(sizeof(t_ast_node *), gcl);
		result[0] = NULL;
		return (result);
	}
}

int	ms_handle_redirections(t_ast_node *node, t_exec_context *context,
		t_proc_manager *manager, t_gc *gcl)
{
	int			old_stdin;
	int			old_stdout;
	t_ast_node	**redir_list;
	size_t		i;

	if (!node)
		return (0);
	old_stdin = context->stdin_fd;
	old_stdout = context->stdout_fd;
	redir_list = redirection_nodes(node, gcl);
	i = 0;
	while (redir_list[i])
	{
		if (redirect_mode(redir_list[i], context, manager, gcl) != 0)
			return (-1);
		i++;
	}
	context->original_stdin = old_stdin;
	context->original_stdout = old_stdout;
	context->redirected = true;
	return (0);
}
