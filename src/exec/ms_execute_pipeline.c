/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:34:19 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/26 18:39:34 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/process.h"

/* We leverage the AST structure -> in a pipeline, pipes will always
 * be right child nodes and commands will always be left child nodes
 * except for the last pipe node that will not have another pipe_node
 * on the right side meaning we have a logic operator or subshell node.
 */

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

static t_ast_node	**st_collect_pipeline_commands(t_ast_node *node,
		int count, t_gc *gcl)
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

static int	**st_prepare_pipes(int num_commands, t_gc *gcl)
{
	int	**pipes;
	int	i;

	if (num_commands <= 1)
		return (NULL);
	pipes = gc_malloc(sizeof(int *) * (num_commands - 1), gcl);
	i = 0;
	while (i < num_commands - 1)
	{
		pipes[i] = gc_malloc(sizeof(int) * 2, gcl);
		if (safe_pipe(pipes[i]) == -1)
			return (NULL);
		i++;
	}
	return (pipes);
}

bool	is_redirect_node(t_ast_node *node)
{
	return (node->type == NODE_REDIRECT_IN
		|| node->type == NODE_REDIRECT_OUT
		|| node->type == NODE_REDIRECT_APPEND
		|| node->type == NODE_REDIRECT_HEREDOC);
}

static void	st_setup_child_io(int i, int num_commands, int **pipes)
{
	int	j;

	if (i > 0 && pipes)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			ft_dprintf(STDERR_FILENO, "minishell: dup2 on STDIN failed");
	}
	if (i < num_commands - 1)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			ft_dprintf(STDERR_FILENO, "minishell: dup2 on STDOUT failed");
	}
	j = 0;
	while (num_commands > 1 && j < num_commands - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

static int	st_child_exit(t_ast_node *final_node,
	t_exec_context *context, t_proc_manager *manager)
{
	int	exit_code;

	if (context->redir_list && ms_apply_redirections(context->redir_list) != 0)
		exit(1);
	exit_code = ms_execute_ast(final_node, context, manager);
	exit(exit_code);
}

static void	st_pipe_process(pid_t *pids, int i, int num_commands,
		t_ast_node *final_node, t_exec_context *context,
		t_proc_manager *manager, int **pipes)
{
	pids[i] = fork();
	if (pids[i] < 0)
	{
		ms_handle_error("minishell: fork failed\n", 1, context->shell->gcl);
		pids[i] = -1;
	}
	else if (pids[i] == 0)
	{
		ms_init_child_cmd_signal();
		st_setup_child_io(i, num_commands, pipes);
		exit(st_child_exit(final_node, context, manager));
	}
	else
		context->redir_list = NULL;
}

pid_t	*ms_fork_pipeline_commands(t_ast_node **commands, int **pipes,
		int num_commands, t_exec_context *context,
		t_proc_manager *manager, t_gc *gcl)
{
	int			i;
	pid_t		*pids;
	t_redir		*redir_list;
	t_ast_node	*final_node;

	i = 0;
	pids = gc_malloc(sizeof(pid_t) * num_commands, gcl);
	while (i < num_commands)
	{
		redir_list = ms_collect_redirections(commands[i], gcl, context->shell);
		context->redir_list = redir_list;
		final_node = commands[i];
		while (final_node && is_redirect_node(final_node))
		{
			if (final_node->type == NODE_REDIRECT_HEREDOC)
				final_node = final_node->data.heredoc.child;
			else
				final_node = final_node->data.redirect.child;
		}
		st_pipe_process(pids, i, num_commands, final_node, context,
			manager, pipes);
		i++;
	}
	return (pids);
}

static void st_close_parent_pipes(int num_commands, int **pipes)
{
	int	i;

	i = 0;
	while (num_commands > 1 && i < num_commands - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

static int st_wait_for_childs(int num_commands, pid_t *pids, t_exec_context *context)
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

int	ms_execute_pipeline(t_ast_node *node, t_exec_context *context,
	t_proc_manager *manager)
{
	t_gc		*gcl;
	int			num_commands;
	t_ast_node	**commands;
	pid_t		*pids;
	int			**pipes;

	gcl = context->shell->gcl;
	num_commands = st_count_pipeline_commands(node);
	commands = st_collect_pipeline_commands(node, num_commands, gcl);
	pipes = st_prepare_pipes(num_commands, gcl);
	pids = ms_fork_pipeline_commands(commands, pipes, num_commands,
			context, manager, gcl);
	st_close_parent_pipes(num_commands, pipes);
	return (st_wait_for_childs(num_commands, pids, context));
}
