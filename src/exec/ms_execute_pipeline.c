/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:34:19 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 11:13:34 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static int	st_count_pipeline_commands(t_ast_node *node)
{
	int			count;
	t_ast_node	*current;

	count = 1;
	current = node;
	while (current->type == NODE_PIPE)
	{
		count++;
		current = current->data.pipe.right;
	}
	return (count);
}

static t_ast_node	**st_collect_pipeline_commands(t_ast_node *node, int count,
	t_gc *gcl)
{
	t_ast_node	**commands;
	t_ast_node	*current;
	int			i;

	commands = gc_malloc(sizeof(t_ast_node *) * (count + 1), gcl);
	current = node;
	i = 0;
	while (current->type == NODE_PIPE && i < count - 1)
	{
		commands[i++] = current->data.pipe.left;
		current = current->data.pipe.right;
	}
	commands[i++] = current;
	commands[i] = NULL;
	return (commands);
}

static void	st_close_parent_pipes(int num_commands, int **pipes)
{
	int	i;

	i = 0;
	while (num_commands > 1 && i < num_commands - 1)
	{
		ms_safe_close(pipes[i][0]);
		ms_safe_close(pipes[i][1]);
		i++;
	}
}

static int	st_wait_for_childs(int num_commands, pid_t *pids,
			t_exec_context *context)
{
	int			i;
	int			last_status;
	int			status;

	i = 0;
	last_status = 0;
	while (i < num_commands)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
		else
			last_status = -1;
		i++;
	}
	context->shell->error_code = last_status;
	return (last_status);
}

int	ms_execute_pipeline(t_ast_node *node, t_exec_context *context)
{
	t_gc		*gcl;
	int			num_commands;
	t_ast_node	**commands;
	pid_t		*pids;
	int			**pipes;

	gcl = context->shell->gcl;
	num_commands = st_count_pipeline_commands(node);
	commands = st_collect_pipeline_commands(node, num_commands, gcl);
	pipes = ms_prepare_pipes(num_commands, gcl);
	pids = ms_fork_pipeline_commands(commands, pipes, num_commands,
			context);
	st_close_parent_pipes(num_commands, pipes);
	return (st_wait_for_childs(num_commands, pids, context));
}
