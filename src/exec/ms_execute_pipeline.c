/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:34:19 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/22 09:58:44 by jeportie         ###   ########.fr       */
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

static t_ast_node **st_collect_pipeline_commands(t_ast_node *node, int count, t_gc *gcl)
{
    t_ast_node **commands;
    t_ast_node *current;
    int i;

    fprintf(stderr, "DEBUG: Collecting pipeline commands\n");
    current = node;
    commands = gc_malloc(sizeof(t_ast_node *) * (count + 1), gcl);
    i = 0;
    while (current->type == NODE_PIPE && i < count - 1)
    {
        fprintf(stderr, "DEBUG: Pipeline node step %d, node type: %d\n", i, current->type);
        commands[i++] = current->data.pipe.left;
        current = current->data.pipe.right;
    }
    fprintf(stderr, "DEBUG: Last pipeline node type: %d\n", current->type);
    commands[i++] = current;
    commands[i] = NULL;
    return (commands);
}

int	ms_execute_pipeline(t_ast_node *node, t_exec_context *context, t_proc_manager *manager)
{
	t_gc		*gcl;
	int			num_commands;
	t_ast_node	**commands;
	pid_t		*pids;
	int			**pipes;
	int			i;

	fprintf(stderr, "DEBUG: Entering ms_execute_pipeline\n");

	gcl = context->shell->gcl;
	num_commands = st_count_pipeline_commands(node);
	fprintf(stderr, "DEBUG: Number of commands in pipeline: %d\n", num_commands);
	commands = st_collect_pipeline_commands(node, num_commands, gcl);
	pids = gc_malloc(sizeof(pid_t) * num_commands, gcl);

	if (num_commands > 1)
	{
		pipes = gc_malloc(sizeof(int *) * (num_commands - 1), gcl);
		i = 0;
		while (i < num_commands - 1)
		{
			pipes[i] = gc_malloc(sizeof(int) * 2, gcl);
			if (safe_pipe(pipes[i]) == -1)
			{
				fprintf(stderr, "DEBUG: Pipe creation failed\n");
				return (ms_handle_error("Pipe failed\n", 1, gcl));
			}
			fprintf(stderr, "DEBUG: Created pipe %d with fds [%d, %d]\n", i, pipes[i][0], pipes[i][1]);
			i++;
		}
	}
	else
	{
		pipes = NULL;
	}

	i = 0;
	while (i < num_commands)
	{
		fprintf(stderr, "DEBUG: Setting up command %d\n", i);
		t_redir *redir_list = ms_collect_redirections(commands[i], gcl, context->shell);
		context->redir_list = redir_list;
		t_ast_node *final_node = commands[i];
		while (final_node && (final_node->type == NODE_REDIRECT_IN
				|| final_node->type == NODE_REDIRECT_OUT
				|| final_node->type == NODE_REDIRECT_APPEND
				|| final_node->type == NODE_REDIRECT_HEREDOC))
		{
			if (final_node->type == NODE_REDIRECT_HEREDOC)
				final_node = final_node->data.heredoc.child;
			else
				final_node = final_node->data.redirect.child;
		}
		fprintf(stderr, "DEBUG: final_node type: %d\n", final_node ? (int)final_node->type : -1);
		if (final_node && final_node->type == NODE_COMMAND)
		{
		    fprintf(stderr, "DEBUG: final_node type: %d\n", final_node ? (int)final_node->type : -1);
		}
		else
		{
		    fprintf(stderr, "DEBUG: final_node is not a command\n");
		}
		fprintf(stderr, "DEBUG: Forking for command %d\n", i);
		pid_t pid = fork();
		if (pid < 0)
		{
			fprintf(stderr, "DEBUG: Fork failed for command %d\n", i);
			return (ms_handle_error("Fork failed\n", 1, gcl));
		}
		if (pid == 0) // Child
		{
			fprintf(stderr, "DEBUG: In child process %d (pid: %d)\n", i, getpid());
			ms_init_child_cmd_signal();

			// Redirect stdin from previous pipe (if any)
			if (i > 0)
			{
				fprintf(stderr, "DEBUG: Command %d duping stdin from pipe %d read end\n", i, i-1);
				if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
					perror("DEBUG: dup2 on STDIN failed");
			}

			// Redirect stdout to next pipe (if any)
			if (i < num_commands - 1)
			{
				fprintf(stderr, "DEBUG: Command %d duping stdout to pipe %d write end\n", i, i);
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
					perror("DEBUG: dup2 on STDOUT failed");
			}

			// Close all pipes in child
			int j = 0;
			while (num_commands > 1 && j < num_commands - 1)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}

			if (context->redir_list && ms_apply_redirections(context->redir_list) != 0)
			{
				fprintf(stderr, "DEBUG: Redirection failed in child for command %d\n", i);
				exit(1);
			}

			fprintf(stderr, "DEBUG: Executing final_node in child %d (pid: %d)\n", i, getpid());
			int exit_code = ms_execute_ast(final_node, context, manager);
			fprintf(stderr, "DEBUG: Child %d (pid: %d) finished with exit_code: %d\n", i, getpid(), exit_code);
			exit(exit_code);
		}
		else // Parent
		{
			fprintf(stderr, "DEBUG: In parent after fork for command %d, child pid: %d\n", i, pid);
			pids[i] = pid;
			context->redir_list = NULL;
		}
		i++;
	}

	// Close all pipes in parent
	i = 0;
	while (num_commands > 1 && i < num_commands - 1)
	{
		fprintf(stderr, "DEBUG: Parent closing pipe %d read and write ends\n", i);
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}

	// Wait for all children
	i = 0;
	int last_status = 0;
	while (i < num_commands)
	{
		int status;
		pid_t w = waitpid(pids[i], &status, 0);
		fprintf(stderr, "DEBUG: Waited for pid %d, status = %d\n", w, status);
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
		else
			last_status = -1;
		i++;
	}

	fprintf(stderr, "DEBUG: Pipeline finished with status %d\n", last_status);
	context->shell->error_code = last_status;
	fprintf(stderr, "DEBUG: Leaving ms_execute_pipeline\n");
	return (last_status);
}

