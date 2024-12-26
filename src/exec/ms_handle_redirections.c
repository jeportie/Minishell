/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_redirections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:12:26 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/26 11:06:03 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/expand.h"
#include <fcntl.h>
#include <unistd.h>

static t_redir	*ms_add_redir(t_redir **head, t_redir_type type,
	char *filename, t_gc *gcl)
{
	t_redir	*new_node;

	new_node = gc_malloc(sizeof(t_redir), gcl);
	new_node->type = type;
	new_node->filename = filename;
	new_node->next = *head;
	*head = new_node;
	return (new_node);
}

static t_redir_type	ms_node_type_to_redir_type(t_node_type ntype)
{
	if (ntype == NODE_REDIRECT_IN)
		return (REDIR_IN);
	else if (ntype == NODE_REDIRECT_OUT)
		return (REDIR_OUT);
	else if (ntype == NODE_REDIRECT_APPEND)
		return (REDIR_APPEND);
	else if (ntype == NODE_REDIRECT_HEREDOC)
		return (REDIR_HEREDOC);
	return (-1);
}

t_redir	*ms_collect_redirections(t_ast_node *node, t_gc *gcl, t_shell *shell)
{
	t_redir		*redir_list;
	t_node_type	ntype;
	char		*filename;

	redir_list = NULL;
	while (node && (node->type == NODE_REDIRECT_IN
			|| node->type == NODE_REDIRECT_OUT
			|| node->type == NODE_REDIRECT_APPEND
			|| node->type == NODE_REDIRECT_HEREDOC))
	{
		ntype = node->type;
		if (ntype == NODE_REDIRECT_HEREDOC)
		{
			ms_add_redir(&redir_list, ms_node_type_to_redir_type(ntype),
				node->data.heredoc.filename, gcl);
			node = node->data.heredoc.child;
		}
		else
		{
			filename = node->data.redirect.filename;
			if (is_var(filename))
				filename = nested_vars(filename, shell);
			ms_add_redir(&redir_list, ms_node_type_to_redir_type(ntype),
				filename, gcl);
			node = node->data.redirect.child;
		}
	}
	return (redir_list);
}

static int	ms_open_redir_file(t_redir_type type, const char *filename)
{
	int		flags;
	int		fd;

	if (type == REDIR_IN)
		flags = O_RDONLY;
	else if (type == REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (type == REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_RDONLY;
	fd = open(filename, flags, COPY_MODE);
	if (fd == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: No such file or directory\n",
			filename);
	}
	return (fd);
}

int	ms_apply_redirections(t_redir *redir_list)
{
	t_redir	*current;
	int		fd;

	current = redir_list;
	while (current)
	{
		fd = ms_open_redir_file(current->type, current->filename);
		if (fd == -1)
		{
			dprintf(STDERR_FILENO, "minishell: Failed to open %s\n", current->filename);
			return (-1);
		}
		if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				dprintf(STDERR_FILENO, "minishell: dup2 error on STDIN\n");
				close(fd);
				return (-1);
			}
			close(fd);
			//if (current->type == REDIR_HEREDOC)
			//	unlink(current->filename);
		}
		else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				dprintf(STDERR_FILENO, "minishell: dup2 error on STDOUT\n");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		current = current->next;
	}
	return (0);
}
