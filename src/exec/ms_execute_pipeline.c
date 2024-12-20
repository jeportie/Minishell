/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:34:19 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/19 23:49:29 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/process.h"

/* We leverage the AST structure -> in a pipeline, pipes will always
 * be right child nodes and commands will always be left child nodes
 * except for the last pipe node that will not have another pipe_node
 * on the right side meaning we have a logic operator or subshell node.
 */

static int	st_count_pipeline_commands(t_pipe_node *pipe_node)
{
	int			count;
	t_ast_node	*current;

	count = 1;
	current = (t_ast_node *)pipe_node;
	while (current->type == NODE_PIPE)
	{
		count++;
		current = current->data.pipe.right;
	}
	return (count);
}

static t_ast_node	**st_collect_pipeline_commands(t_pipe_node *pipe_node, int count, t_gc *gcl)
{
	t_ast_node	**commands;
	t_ast_node	*current;
	int			i;

	current = (t_ast_node *)pipe_node;
	commands = gc_malloc(sizeof(t_ast_node *) * (count + 1), gcl);
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

static pid_t	*st_setup_pipes_and_exec(int num_commands, int pipefd[2], t_gc *gcl)
{
	int		i;
	pid_t	*pids;
	int		in_fd;

	in_fd = STDIN_FILENO;
	i = 0;
	pids = gc_malloc(sizeof(pid_t) * num_commands, gcl);
	while (i < num_commands)
	{
		if (i < num_commands - 1)
		{
			if (safe_pipe(pipefd) == -1)
				return (NULL);
		}
		else
		{
			pipefd[0] = STDIN_FILENO;
			pipefd[1] = STDOUT_FILENO;
		}
		pids[i] = execute_command();
		if (pids[i] < 0)
		{
			ft_dprintf(STDERR_FILENO, "Failed to execute command.\n");
			free(pids);
			return (NULL);
		}
		if (pipefd[1] != STDOUT_FILENO)
			close(pipefd[1]);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		in_fd = pipefd[0];
	}
	return (pids);
}

static	int	st_wait_all_childs(int num_commands, pid_t *pids, t_gc *gcl)
{
	int	i;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < num_commands)
	{
		int	status;
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
		i++;
	}
	gc_collect(gcl);
	return (last_status);
}

int	ms_execute_pipeline(t_pipe_node *pipe_node, t_exec_context *context, t_proc_manager *manager)
{
	int			num_commands;
	t_ast_node	**commands;
	pid_t		*pids;
	int			pipefd[2];

	num_commands = st_count_pipeline_commands(pipe_node);
	commands = st_collect_pipeline_commands(pipe_node, num_commands, context->shell->gcl);
	pids = st_setup_pipes_and_exec(num_commands, pipefd, context->shell->gcl);
	return (st_wait_all_childs(num_commands, pids, context->shell->gcl));
}

///////////////////////////////////////////////////////////////////////////////
/* OLD RECURSIVE VERSION
static	void	init_left_fork_params(t_fork_params *fork_params,
	t_exec_context *context, t_pipe_exec_params params)
{
	fork_params->child_lvl = context->child_lvl + 1;
	fork_params->fd_in = context->stdin_fd;
	fork_params->fd_out = params.pipefd[1];
	fork_params->fd_error = context->stderr_fd;
	fork_params->is_heredoc = false;
	fork_params->title = "L-Pipe";
}

static	void	init_right_fork_params(t_fork_params *fork_params,
	t_exec_context *context, t_pipe_exec_params params)
{
	fork_params->child_lvl = context->child_lvl + 1;
	fork_params->fd_in = params.pipefd[0];
	fork_params->fd_out = context->stdout_fd;
	fork_params->fd_error = context->stderr_fd;
	fork_params->is_heredoc = false;
	fork_params->title = "R-Pipe";
}

void	init_helper(t_pipe_exec_params *params, t_pipe_node *pipe_node,
			t_exec_context *context, t_proc_manager *manager)
{
	params->pipe_node = pipe_node;
	params->context = context;
	params->manager = manager;
	safe_pipe(params->pipefd);
}

int	ms_execute_pipelines(t_pipe_node *pipe_node, t_exec_context *context,
	t_proc_manager *manager)
{
	t_pipe_exec_params	params;
	pid_t				left_pid;
	pid_t				right_pid;
	t_fork_params		fork_params;

	init_helper(&params, pipe_node, context, manager);
	init_left_fork_params(&fork_params, context, params);
	left_pid = safe_fork(manager, &fork_params);
	if (left_pid == 0)
	{
		context->child_lvl = fork_params.child_lvl;
		left_child_process(&params);
	}
	print_proc_info(manager);
	init_right_fork_params(&fork_params, context, params);
	right_pid = safe_fork(manager, &fork_params);
	if (right_pid == 0)
	{
		context->child_lvl = fork_params.child_lvl;
		right_child_process(&params);
	}
	print_proc_info(manager);
	safe_close(params.pipefd[0]);
	safe_close(params.pipefd[1]);
	return (parent_process(left_pid, right_pid, context));
}
*/
///////////////////////////////////////////////////////////////////////////
