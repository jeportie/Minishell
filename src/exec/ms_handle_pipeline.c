/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_pipeline.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:21:40 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 13:50:06 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	**prepare_pipes(int num_commands, t_gc *gcl)
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

static int	st_child_exit(t_ast_node *final_node, t_exec_context *context)
{
	int	exit_code;

	if (context->redir_list && ms_apply_redirections(context->redir_list) != 0)
		exit(1);
	exit_code = ms_execute_ast(final_node, context);
	gc_cleanup(context->shell->gcl);
	exit(exit_code);
}

void	pipe_process(t_pipe_helper *args, t_ast_node *final_node,
				t_exec_context *context, int **pipes)
{
	args->pids[args->i] = fork();
	if (args->pids[args->i] < 0)
	{
		ms_handle_error("minishell: fork failed\n", 1, context->shell->gcl);
		args->pids[args->i] = -1;
	}
	else if (args->pids[args->i] == 0)
	{
		ms_init_child_cmd_signal();
		st_setup_child_io(args->i, args->num_commands, pipes);
		exit(st_child_exit(final_node, context));
	}
	else
		context->redir_list = NULL;
}

pid_t	*ms_fork_pipeline_commands(t_ast_node **commands, int **pipes,
		int num_commands, t_exec_context *context)
{
	t_pipe_helper	args;
	t_redir			*redir_list;
	t_ast_node		*final_node;

	args.i = 0;
	args.pids = gc_malloc(sizeof(pid_t) * num_commands, context->shell->gcl);
	args.num_commands = num_commands;
	while (args.i < num_commands)
	{
		redir_list = ms_collect_redirections(commands[args.i],
				context->shell->gcl, context->shell);
		context->redir_list = redir_list;
		final_node = commands[args.i];
		while (final_node && is_redirect_node(final_node))
		{
			if (final_node->type == NODE_REDIRECT_HEREDOC)
				final_node = final_node->data.heredoc.child;
			else
				final_node = final_node->data.redirect.child;
		}
		pipe_process(&args, final_node, context, pipes);
		args.i++;
	}
	return (args.pids);
}
