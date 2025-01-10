/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:17:41 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 15:33:08 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include "../../include/minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>

t_ast_node	*get_left_child(t_ast_node *node)
{
	if (node == NULL)
		return (NULL);
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (node->data.logic.left);
	else if (node->type == NODE_PIPE)
		return (node->data.pipe.left);
	else if (node->type == NODE_REDIRECT_OUT || node->type == NODE_REDIRECT_IN
		|| node->type == NODE_REDIRECT_APPEND)
		return (node->data.redirect.child);
	else if (node->type == NODE_REDIRECT_HEREDOC)
		return (node->data.heredoc.child);
	else if (node->type == NODE_SUBSHELL)
		return (node->data.subshell.child);
	else
		return (NULL);
}

t_ast_node	*get_right_child(t_ast_node *node)
{
	if (node == NULL)
		return (NULL);
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (node->data.logic.right);
	else if (node->type == NODE_PIPE)
		return (node->data.pipe.right);
	else
		return (NULL);
}

char	*get_node_label(t_ast_node *node)
{
	if (node == NULL)
		return ("");
	if (node->type == NODE_COMMAND)
		return ("\033[1;32m[CMD]\033[0m");
	else if (node->type == NODE_AND)
		return ("\033[1;34m{&&}\033[0m");
	else if (node->type == NODE_OR)
		return ("\033[1;34m{||}\033[0m");
	else if (node->type == NODE_PIPE)
		return ("\033[1;34m{|}\033[0m");
	else if (node->type == NODE_REDIRECT_OUT)
		return ("\033[1;33m>\033[0m");
	else if (node->type == NODE_REDIRECT_IN)
		return ("\033[1;33m<\033[0m");
	else if (node->type == NODE_REDIRECT_APPEND)
		return ("\033[1;33m>>\033[0m");
	else if (node->type == NODE_REDIRECT_HEREDOC)
		return ("\033[1;33m<<\033[0m");
	else if (node->type == NODE_SUBSHELL)
		return ("\033[1;33m()\033[0m");
	else
		return ("[UNKNOWN]");
}

void	print_node_content(t_ast_node *node, int fd)
{
	int			i;
	t_cmd_node	cmd;

	if (node == NULL)
		return ;
	if (node->type == NODE_COMMAND)
	{
		cmd = node->data.command;
		ft_dprintf(fd, " (");
		i = 0;
		while (i < cmd.argc)
		{
			ft_dprintf(fd, "%s", cmd.argv[i]);
			if (i < cmd.argc - 1)
				ft_dprintf(fd, ", ");
			i++;
		}
		ft_dprintf(fd, ")");
	}
	else if (node->type == NODE_REDIRECT_OUT || node->type == NODE_REDIRECT_IN
		|| node->type == NODE_REDIRECT_APPEND)
		ft_dprintf(fd, " (File: %s)", node->data.redirect.filename);
	else if (node->type == NODE_REDIRECT_HEREDOC)
		ft_dprintf(fd, " (Delimiter: %s)", node->data.heredoc.delimiter);
}

void	print_ast(t_ast_node *node, int depth, char *prefix, int is_left)
{
	char (new_prefix)[256] = {0, };
	if (node == NULL)
		return ;
	int (fd) = open(PRINT_INFOS, O_WRONLY | O_APPEND, COPY_MODE);
	ft_dprintf(fd, "%s", prefix);
	print_branch(depth, is_left, fd);
	ft_dprintf(fd, "%s", get_node_label(node));
	print_node_content(node, fd);
	ft_dprintf(fd, "\n");
	int (single_branch) = (node->type == NODE_REDIRECT_OUT || node->type
			== NODE_REDIRECT_IN || node->type == NODE_REDIRECT_APPEND
			|| node->type == NODE_REDIRECT_HEREDOC
			|| node->type == NODE_SUBSHELL);
	if (!single_branch && is_left)
		ft_snprintf(new_prefix, sizeof(new_prefix), "%s│   ", prefix);
	else
		ft_snprintf(new_prefix, sizeof(new_prefix), "%s    ", prefix);
	if (!single_branch)
	{
		print_ast(get_left_child(node), depth + 1, new_prefix, 1);
		print_ast(get_right_child(node), depth + 1, new_prefix, 0);
	}
	else
		print_ast(get_left_child(node), depth + 1, new_prefix, 0);
	close(fd);
}
