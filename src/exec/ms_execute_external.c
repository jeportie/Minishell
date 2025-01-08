/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_external.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 12:43:01 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/08 11:37:15 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include <sys/stat.h>

static int	is_directory(const char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}

static void	ms_child_process(t_cmd_node *cmd_node, t_exec_context *context,
			char *cmd_path, t_gc *gcl)
{
	char	**envp;
	t_gc	*child_gcl;

	ms_init_child_cmd_signal();
	child_gcl = gc_init();
	if (!init_io(context->stdin_fd, context->stdout_fd, context->stderr_fd))
		exit(ms_handle_error("minishell: redirection error: dup2\n",
				EXIT_FAILURE, gcl));
	envp = ms_get_envp(context->shell->env_data->env, child_gcl);
	if (!envp)
		exit(ms_handle_error("memory allocation error\n", EXIT_FAILURE, gcl));
	gc_nest_register(envp, child_gcl);
	if (is_directory(cmd_path))
	{
		ft_dprintf(2, SHELL ": %s: is a directory\n", cmd_path);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, cmd_node->argv, envp);
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
		context->shell->error_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		context->shell->error_code = 128 + WTERMSIG(status);
	else
		context->shell->error_code = -1;
	context->redir_list = NULL;
}

int	ms_execute_external(t_cmd_node *cmd_node, t_exec_context *context,
			t_gc *gcl)
{
	char *(cmd_path) = ms_parse_cmd_path(cmd_node->argv[0], context->shell);
	if (cmd_path == NULL)
	{
		if (ft_strchr(cmd_node->argv[0], '/'))
			ft_dprintf(STDERR_FILENO, "bash: %s: No such file or directory\n",
				cmd_node->argv[0]);
		else
			ft_dprintf(STDERR_FILENO, "bash: %s: command not found\n",
				cmd_node->argv[0]);
		context->shell->error_code = 127;
		return (context->shell->error_code);
	}
	pid_t (pid) = fork();
	ms_init_parent_cmd_signal();
	if (pid == 0)
	{
		if (context->redir_list
			&& ms_apply_redirections(context->redir_list) != 0)
			exit(1);
		ms_child_process(cmd_node, context, cmd_path, gcl);
	}
	else
		ms_parent_process(pid, context);
	return (ms_init_std_signal(), context->shell->error_code);
}
