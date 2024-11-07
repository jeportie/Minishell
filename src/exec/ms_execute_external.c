/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_external.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 12:43:01 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 19:43:59 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/process.h"

static void	init_io(t_exec_context *context)
{
	if (context->stdin_fd != STDIN_FILENO)
	{
		if (redirect_fd(STDIN_FILENO, context->stdin_fd) == -1)
		{
			ft_dprintf(STDERR_FILENO, "redirect_fd failed (stdin)\n");
			exit(EXIT_FAILURE);
		}
	}
	if (context->stdout_fd != STDOUT_FILENO)
	{
		if (redirect_fd(STDOUT_FILENO, context->stdout_fd) == -1)
		{
			ft_dprintf(STDERR_FILENO, "redirect_fd failed (stdout)\n");
			exit(EXIT_FAILURE);
		}
	}
	if (context->stderr_fd != STDERR_FILENO)
	{
		if (redirect_fd(STDERR_FILENO, context->stderr_fd) == -1)
		{
			ft_dprintf(STDERR_FILENO, "redirect_fd failed (stderr)\n");
			exit(EXIT_FAILURE);
		}
	}
}

static void	ms_child_process(t_cmd_node *cmd_node, t_exec_context *context,
			char *cmd_path, t_gc *gcl)
{
	char	**envp;

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
		safe_close(context->stdin_fd);
	if (context->stdout_fd != STDOUT_FILENO)
		safe_close(context->stdout_fd);
	if (context->stderr_fd != STDERR_FILENO)
		safe_close(context->stderr_fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		context->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		context->exit_status = 128 + WTERMSIG(status);
	else
		context->exit_status = -1;
}

static void	init_forks(t_fork_params *fork_params, t_exec_context *context,
	t_cmd_node *cmd_node)
{
	fork_params->child_lvl = context->child_lvl + 1;
	fork_params->fd_in = context->stdin_fd;
	fork_params->fd_out = context->stdout_fd;
	fork_params->fd_error = context->stderr_fd;
	fork_params->is_heredoc = false;
	fork_params->title = cmd_node->argv[0];
}

int	ms_execute_external(t_cmd_node *cmd_node, t_exec_context *context,
		t_proc_manager *manager, t_gc *gcl)
{
	pid_t			pid;
	char			*cmd_path;
	t_fork_params	fork_params;

	cmd_path = ms_parse_cmd_path(cmd_node->argv[0], context->shell);
	if (cmd_path == NULL)
	{
		ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd_node->argv[0], STDERR_FILENO);
		return (127);
	}
	init_forks(&fork_params, context, cmd_node);
	pid = safe_fork(manager, &fork_params);
	if (pid == 0)
	{
		context->child_lvl = fork_params.child_lvl;
		ms_child_process(cmd_node, context, cmd_path, gcl);
	}
	else
	{
		print_proc_info(manager);
		ms_parent_process(pid, context);
	}
	return (context->exit_status);
}
