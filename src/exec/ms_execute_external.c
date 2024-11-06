/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_external.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 12:43:01 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 11:28:24 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include <unistd.h>

static void	safe_dup2(const char *error_message, int context_fd, int std_fd)
{
	if (dup2(context_fd, std_fd) == -1)
	{
		ft_dprintf(std_fd, error_message);
		exit(EXIT_FAILURE);
	}
}

static void	init_io(t_exec_context *context)
{
	if (context->stdin_fd != STDIN_FILENO)
		safe_dup2("minishell: error: dup2 failed (stdin)\n",
			context->stdin_fd, STDIN_FILENO);
	if (context->stdout_fd != STDOUT_FILENO)
		safe_dup2("minishell: error: dup2 failed (stdout)\n",
			context->stdout_fd, STDOUT_FILENO);
	if (context->stderr_fd != STDERR_FILENO)
		safe_dup2("minishell: error: dup2 failed (stderr)\n",
			context->stderr_fd, STDERR_FILENO);
	if (context->stdin_fd != STDIN_FILENO)
		ms_redirect_input(context->stdin_fd);
	if (context->stdout_fd != STDOUT_FILENO)
		ms_redirect_output(context->stdout_fd);
}

static void	ms_child_process(t_cmd_node *cmd_node, t_exec_context *context,
			char *cmd_path)
{
	char	**envp;
	t_gc	*gcl;

	gcl = gc_init();
	init_io(context);
	envp = ms_get_envp(context->shell->env_data->env, context->shell->gcl);
	if (!envp)
	{
		perror("minishell: memory allocation error");
		gc_cleanup(gcl);
		exit(EXIT_FAILURE);
	}
	gc_nest_register(envp, gcl);
	gc_nest_lock(envp, gcl);
	execve(cmd_path, cmd_node->argv, envp);
	perror("minishell: execve error");
	gc_cleanup(gcl);
	exit(EXIT_FAILURE);
}

static void	ms_parent_process(pid_t pid, t_exec_context *context)
{
	int	status;

	if (context->stdin_fd != STDIN_FILENO)
		close(context->stdin_fd);
	if (context->stdout_fd != STDOUT_FILENO)
		close(context->stdout_fd);
	if (context->stderr_fd != STDERR_FILENO)
		close(context->stderr_fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		context->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		context->exit_status = 128 + WTERMSIG(status);
	else
		context->exit_status = -1;
}

int	ms_execute_external(t_cmd_node *cmd_node, t_exec_context *context,
	t_gc *gcl)
{
	pid_t	pid;
	char	*cmd_path;

	cmd_path = ms_parse_cmd_path(cmd_node->argv[0], context->shell);
	if (cmd_path == NULL)
	{
		ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd_node->argv[0], STDERR_FILENO);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork error");
		gc_cleanup(gcl);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		ms_child_process(cmd_node, context, cmd_path);
	else
		ms_parent_process(pid, context);
	return (context->exit_status);
}
