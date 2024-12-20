/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_ast.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 09:32:42 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/20 15:00:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

bool is_builtin_that_must_run_in_parent(t_cmd_node *cmd_node)
{
    if (!cmd_node || !cmd_node->argv[0])
        return false;
    if (ft_strncmp(cmd_node->argv[0], "cd", 3) == 0)
        return true;
    if (ft_strncmp(cmd_node->argv[0], "export", 7) == 0)
        return true;
    if (ft_strncmp(cmd_node->argv[0], "unset", 6) == 0)
        return true;
    if (ft_strncmp(cmd_node->argv[0], "exit", 5) == 0)
        return true;
    return false;
}

int	ms_execute_ast(t_ast_node *node, t_exec_context *context,
	t_proc_manager *manager)
{
	if (!node)
		return (ms_handle_error("Error: Null AST node.\n", -1, context->shell->gcl));
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (ms_execute_logical(node, context, manager));
	else if (node->type == NODE_SUBSHELL)
		return (ms_execute_subshell(&node->data.subshell, context, manager));
	else if (node->type == NODE_PIPE)
		return (ms_execute_pipeline(&node->data.pipe, context, manager));
	else if (node->type == NODE_COMMAND)
		return (ms_execute_command(&node->data.command, context, manager, context->shell->gcl));
	else
    {
        t_redir *redir_list = ms_collect_redirections(node, context->shell->gcl, context->shell);
        while (node && (node->type == NODE_REDIRECT_IN || node->type == NODE_REDIRECT_OUT
            || node->type == NODE_REDIRECT_APPEND || node->type == NODE_REDIRECT_HEREDOC))
        {
            if (node->type == NODE_REDIRECT_HEREDOC)
                node = node->data.heredoc.child;
            else
                node = node->data.redirect.child;
        }
        if (!node)
            return ms_handle_error("Redirection with no command\n", 1, context->shell->gcl);
        if (node->type == NODE_COMMAND)
        {
            context->redir_list = redir_list;
        
            // Save original file descriptors
            int saved_stdin = dup(STDIN_FILENO);
            int saved_stdout = dup(STDOUT_FILENO);
        
            // Apply redirections for ANY command before executing it
            if (ms_apply_redirections(context->redir_list) != 0)
            {
                dup2(saved_stdin, STDIN_FILENO);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdin);
                close(saved_stdout);
                context->redir_list = NULL;
                return 1;
            }
        
            int ret = ms_execute_command(&node->data.command, context, manager, context->shell->gcl);
        
            // Restore original file descriptors
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
            context->redir_list = NULL;
        
            return ret;
        }
        else
        {
            context->redir_list = redir_list;
            return ms_execute_ast(node, context, manager);
        }
    }
}
