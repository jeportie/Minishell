/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:04:44 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/08 14:11:15 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include "../../include/minishell.h"

static int	var_len(char *arg, t_gc *gcl)
{
	int (i) = 0;
	int (len) = 0;
	if (!ft_strncmp(arg, "?", 2))
		return (1);
	if (arg[i] == '{')
	{
		i++;
		while (arg[i] && arg[i] != '}')
		{
			len++;
			i++;
		}
		if (arg[i] != '}')
			ms_handle_error("minishell: error: "
				"unexpected token near {", 127, gcl);
	}
	else
	{
		while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
		{
			len++;
			i++;
		}
	}
	return (len);
}

static char	*ms_extract_var(char *arg, t_gc *gcl)
{
	int (i) = 0;
	int (j) = 0;
	int (len) = var_len(arg, gcl);
	char *(var) = (char *)gc_malloc(sizeof(char) * (len + 1), gcl);
	if (arg[i] == '{')
	{
		i++;
		while (arg[i] && arg[i] != '}')
			var[j++] = arg[i++];
	}
	else if (len == 1 && !ft_strncmp(arg, "?", 2))
		var[j++] = '?';
	else
		while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
			var[j++] = arg[i++];
	return (var[j] = '\0', var);
}

static void	helper_copy(t_expand_utils *ex_utils, char *arg, int *old_arg_index,
		int new_arg_index)
{
	while (arg[new_arg_index] && (ft_isalnum(arg[new_arg_index])
			|| arg[new_arg_index] == '-'))
		new_arg_index++;
	while (arg[new_arg_index])
	{
		if (arg[new_arg_index] == '{')
		{
			while (arg[new_arg_index] != '}' && arg[new_arg_index])
				new_arg_index++;
			if (arg[new_arg_index] == '}')
				new_arg_index++;
		}
		if (*old_arg_index < ex_utils->total_len)
		{
			ex_utils->new_arg[*old_arg_index] = arg[new_arg_index];
			(*old_arg_index)++;
		}
		new_arg_index++;
	}
	ex_utils->new_arg[*old_arg_index] = '\0';
}

static void	copy_expanded_cmd(char *arg, bool is_nested,
		t_expand_utils *ex_utils)
{
	int (arg_index) = 0;
	int (expand_index) = 0;
	int (i) = 0;
	if (!is_nested)
	{
		while (arg[arg_index] && arg[arg_index] != '$')
		{
			ex_utils->new_arg[arg_index] = arg[arg_index];
			arg_index++;
		}
	}
	expand_index = 0;
	i = arg_index + 1;
	while (expand_index < ex_utils->total_len)
	{
		ex_utils->new_arg[arg_index] = ex_utils->expand_var[expand_index];
		arg_index++;
		expand_index++;
	}
	helper_copy(ex_utils, arg, &arg_index, i);
}

char	*ms_expand_arg(char *arg, t_shell *shell, bool is_nested)
{
	t_expand_utils	ex_utils;

	ex_utils.total_len = 0;
	if (!is_nested)
		ex_utils.start_var = find_dollar(arg);
	else
		ex_utils.start_var = arg;
	ex_utils.var = ms_extract_var(ex_utils.start_var, shell->gcl);
	ex_utils.expand_var = ms_get_env_value(shell->env_data->env,
			ex_utils.var, shell->error_code);
	if (!ex_utils.expand_var)
		return (NULL);
	ex_utils.total_len = ft_strlen(arg) + ft_strlen(ex_utils.expand_var)
		- ft_strlen(ex_utils.var);
	ex_utils.new_arg = (char *)gc_malloc(sizeof(char) * (ex_utils.total_len
				+ 1), shell->gcl);
	if (!ex_utils.new_arg)
		ms_handle_error("minishell: memory allocation error", 127, shell->gcl);
	copy_expanded_cmd(arg, is_nested, &ex_utils);
	if (is_var(ex_utils.new_arg))
		ex_utils.new_arg = nested_vars(ex_utils.new_arg, shell);
	return (ex_utils.new_arg);
}
