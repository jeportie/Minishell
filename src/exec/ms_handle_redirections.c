/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_redirections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:12:26 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/06 14:53:28 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/expand.h"

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

static bool	st_is_redir(t_ast_node *node)
{
	if (node->type == NODE_REDIRECT_IN || node->type == NODE_REDIRECT_OUT
		|| node->type == NODE_REDIRECT_APPEND
		|| node->type == NODE_REDIRECT_HEREDOC)
		return (true);
	return (false);
}

t_redir	*ms_collect_redirections(t_ast_node *node, t_gc *gcl, t_shell *shell)
{
	t_redir		*redir_list;
	t_node_type	ntype;
	char		*filename;

	redir_list = NULL;
	while (node && st_is_redir(node))
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

int	ms_open_redir_file(t_redir_type type, const char *filename)
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
		ft_dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n",
			SHELL, filename);
	}
	return (fd);
}
