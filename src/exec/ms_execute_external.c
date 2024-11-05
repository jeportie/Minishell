/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_external.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 12:43:01 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/05 18:12:06 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	ms_redirect_input(int in_fd)
{
	if (dup2(in_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 error (input)");
		exit(EXIT_FAILURE);
	}
	close(in_fd);
}

void	ms_redirect_output(int out_fd)
{
	if (dup2(out_fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 error (output)");
		exit(EXIT_FAILURE);
	}
	close(out_fd);
}

static void	ms_child_process(t_cmd_node *cmd_node, t_exec_context *context,
			char *cmd_path)
{
	char	**envp;
	t_gc	*gcl;

	gcl = gc_init();
	if (context->stdin_fd != STDIN_FILENO)
    {
        if (dup2(context->stdin_fd, STDIN_FILENO) == -1)
        {
            perror("minishell: dup2 failed (stdin)");
            exit(EXIT_FAILURE);
        }
    }
    if (context->stdout_fd != STDOUT_FILENO)
    {
        if (dup2(context->stdout_fd, STDOUT_FILENO) == -1)
        {
            perror("minishell: dup2 failed (stdout)");
            exit(EXIT_FAILURE);
        }
    }
    if (context->stderr_fd != STDERR_FILENO)
    {
        if (dup2(context->stderr_fd, STDERR_FILENO) == -1)
        {
            perror("minishell: dup2 failed (stderr)");
            exit(EXIT_FAILURE);
        }
    }
	if (context->stdin_fd != STDIN_FILENO)
		ms_redirect_input(context->stdin_fd);
	if (context->stdout_fd != STDOUT_FILENO)
		ms_redirect_output(context->stdout_fd);
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

int	ms_execute_external(t_cmd_node *cmd_node, t_exec_context *context, t_gc *gcl)
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
