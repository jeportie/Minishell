/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:17:07 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/13 19:03:08 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/builtins.h"

bool	is_var(char *cmd)
{
	int	i;

	if (!cmd)
		return (false);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '$')
			return (true);
		i++;
	}
	return (false);
}

bool	is_wild(char *cmd)
{
	int	i;

	if (!cmd)
		return (false);
	while (cmd[i])
	{
		if (cmd[i] == '*')
			return (true);
		i++;
	}
	return (false);
}

bool	is_equal(char *cmd)
{
	int	i;

	if (!cmd)
		return (false);
	while (cmd[i])
	{
		if (cmd[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

int	valide_dollar(char *input)
{
	int	i;

	i = 0;
	if (ft_isdigit(input[i]))
	{
		ft_dprintf(2, "minishell: %s\':"
			" command not found\n", input);
		return (0);
	}
	while (input[i])
	{
		if (ft_isalnum(input[i]) || input[i] == '_')
			i++;
		else if ((input[i] == '=') || (input[i] == '+' && input[i + 1] == '='))
			return (i);
		else
		{
			ft_dprintf(2, "minishell: %s\':"
				" command not found\n", input);
			return (0);
		}
	}
	return (i);
}

int	ms_value_assign(t_shell *shell, char *cmd, t_gc *gcl)
{
	char	*var;
	char	*value;
	int		i;
	int		j;
	int		len;

	if (!cmd)
		return (-1);
	else
	{
		i = 0;
		len = valide_dollar(cmd);
		if (cmd[len] == '=')
		{
			var = (char *)gc_malloc(sizeof(char) * (len + 1), gcl);
			i++;
			len = 0;
			while (cmd[i])
			{
				i++;
				len++;
			}
			// value can also have an expand or wildcard in it !
			value = (char *)gc_malloc(sizeof(char) * (len + 1), gcl);
			i = 0;
			while (cmd[i] != '=')
			{
				var[i] = cmd[i];
				i++;
			}
			var[i] = '\0';
			i++;
			j = 0;
			while (cmd[i])
			{
				value[j] = cmd[i];
				i++;
				j++;
			}
			value[j] = '\0';
			ms_set_env_value(shell, var, value);	
			return (0);
		}
		else
			return (127);
	}
	return (127);
}

char	*ms_expand_arg(char *arg, t_env *env, t_gc *gcl)
{
	char	*new_arg;
	char	*var;
	char	*expand_var;
	int		i;
	int		j;
	int		h;
	int		total_len;
	int		var_len;

	i = 0;
	total_len = 0;
	var_len = 0;
	while (arg[i] && arg[i] != '$')
	{
		i++;
		total_len++;
	}
	printf("$ is %i away from first char.\n", i);
	i++;
	j = i;
	while (arg[j] && (ft_isalnum(arg[j]) || arg[j] == '-'))
	{
		j++;
		var_len++;
	}
	var = (char *)gc_malloc(sizeof(char) * (var_len + 1), gcl);
	j = 0;
	while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '-'))
	{
		var[j] = arg[i];
		i++;
		j++;
	}
	var[j] = '\0';
	expand_var = (char *)gc_malloc(sizeof(char)
			* (ft_strlen(ms_get_env_value(env, var)) + 1), gcl);
	expand_var = ms_get_env_value(env, var);
	total_len += ft_strlen(ms_get_env_value(env, var));
	while (arg[i])
	{
		i++;
		total_len++;
	}
	printf("total len = %d\n", total_len);
	new_arg = (char *)gc_malloc(sizeof(char) * (total_len + 1), gcl);
	i = 0;
	while (arg[i] && arg[i] != '$')
	{
		new_arg[i] = arg[i];
		i++;
	}
	j = 0;
	h = i + 1;
	while (expand_var[j])
	{
		new_arg[i] = expand_var[j];
		i++;
		j++;
	}
	while (arg[h] && (ft_isalnum(arg[h]) || arg[h] == '-'))
		h++;
	while (arg[h])
	{
		new_arg[i] = arg[h];
		i++;
		h++;
	}
	new_arg[i] = '\0';
	gc_free(arg, gcl);
	return (new_arg);
}

int	ms_execute_command(t_cmd_node *cmd_node, t_exec_context *context,
	t_proc_manager *manager, t_gc *gcl)
{
	char	*cmd;
	int		i;

	cmd = cmd_node->argv[0];
	i = 1;
	while (i < cmd_node->argc)
	{
		if (is_var(cmd_node->argv[i]))
		{
			cmd_node->argv[i] = ms_expand_arg(cmd_node->argv[i],
					context->shell->env_data->env, gcl);
		}
		i++;
	}
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (ms_echo(cmd_node));
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (ms_cd(cmd_node, context));
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (ms_pwd());
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (ms_export(cmd_node, context));
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (ms_unset(cmd_node, context));
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (ms_env(cmd_node, context));
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (ms_exit(cmd_node, context));
	else if (is_equal(cmd))
		return(ms_value_assign(context->shell, cmd, gcl));
	else
		return (ms_execute_external(cmd_node, context, manager, gcl));
}
