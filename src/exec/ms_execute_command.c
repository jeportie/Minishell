/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:07:30 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/14 18:29:13 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/builtins.h"

/*
 * This function will check if there is a nested $ in the command, expand
 * one by one each $ en append all together to it return value
 * example a=jer b=ome if we do echo $a$b it will append jer and ome in the
 * return
 */

int	count_dollars(char *arg)
{
	int	i;
	int	dollars;

	if (!arg)
		return (0);
	i = 0;
	dollars = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
			dollars++;
		i++;
	}
	return (dollars);
}

char	*ft_strsjoin(int argc, char **argv, t_gc *gcl)
{
	char	*result;
	int		result_index;
	int		i;
	int		j;
	int		len;

	if (!argv || !*argv)
		return (NULL);
	i = 0;
	len = 0;
	while (i < argc)
	{
		len += ft_strlen(argv[i]);
		i++;
	}
	result = gc_malloc(sizeof(char) * (len + 1), gcl);
	gc_lock(result, gcl);
	i = 0;
	j = 0;
	result_index = 0;
	while (i < argc)
	{
		while (argv[i][j])
		{
			result[result_index] = argv[i][j];
			result_index++;
			j++;
		}
		j = 0;
		i++;
	}
	result[result_index] = '\0';
	return (result);
}

char	*nested_vars(char *arg, t_env *env, t_gc *gcl)
{
	char	*result;
	char	**vars;
	char	**expanded_vars;
	int		dollars;
	int		i;

	dollars = count_dollars(arg);
	if (dollars < 2)
		return (ms_expand_arg(arg, env, false, gcl));
	else
	{
		vars = ft_split(arg, '$');
		gc_nest_register(vars, gcl);
		gc_lock(vars, gcl);
		expanded_vars = gc_malloc(sizeof(char *) * (dollars + 1), gcl);
		i = 0;
		while (i < dollars)
		{
			expanded_vars[i] = ms_expand_arg(vars[i], env, true, gcl);
			i++;
		}
		expanded_vars[i] = NULL;
	}
	result = ft_strsjoin(dollars, expanded_vars, gcl);
	return (result);
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
			cmd_node->argv[i] = nested_vars(cmd_node->argv[i],
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
		return (ms_value_assign(context->shell, cmd_node, gcl));
	else
		return (ms_execute_external(cmd_node, context, manager, gcl));
}
