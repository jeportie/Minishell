/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:04:44 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/19 14:10:54 by jeportie         ###   ########.fr       */
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
		{
			var[j] = arg[i];
			i++;
			j++;
		}
	}
	else if (len == 1 && !ft_strncmp(arg, "?", 2))
	{
		var[j] = '?';
		j++;
	}
	else
	{
		while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
		{
			var[j] = arg[i];
			i++;
			j++;
		}
	}
	var[j] = '\0';
	return (var);
}

static void	helper_copy(char *new_arg, char *arg, int *old_arg_index,
		int new_arg_index)
{
	while (arg[new_arg_index] && (ft_isalnum(arg[new_arg_index])
			|| arg[new_arg_index] == '-'))
		new_arg_index++;
	while (arg[new_arg_index])
	{
		if (arg[new_arg_index] == '{')
		{
			while (arg[new_arg_index] != '}')
				new_arg_index++;
			new_arg_index++;
		}
		if (arg[new_arg_index] == '}')
			new_arg_index++;
		new_arg[*old_arg_index] = arg[new_arg_index];
		(*old_arg_index)++;
		new_arg_index++;
	}
	new_arg[*old_arg_index] = '\0';
}

static void	copy_expanded_cmd(char *new_arg, char *arg,
		char *expand_var, bool is_nested)
{
	int	arg_index;
	int	expand_index;
	int	i;

	arg_index = 0;
	expand_index = 0;
	if (!is_nested)
	{
		while (arg[arg_index] && arg[arg_index] != '$')
		{
			new_arg[arg_index] = arg[arg_index];
			arg_index++;
		}
	}
	expand_index = 0;
	i = arg_index + 1;
	while (expand_var[expand_index])
	{
		new_arg[arg_index] = expand_var[expand_index];
		arg_index++;
		expand_index++;
	}
	helper_copy(new_arg, arg, &arg_index, i);
}

char	*ms_expand_arg(char *arg, t_shell *shell, bool is_nested)
{
	char	*new_arg;
	char	*var;
	char	*expand_var;
	char	*start_var;
	int		total_len;

	total_len = 0;
	if (!is_nested)
		start_var = find_dollar(arg);
	else
		start_var = arg;
	var = ms_extract_var(start_var, shell->gcl);
	expand_var = ms_get_env_value(shell->env_data->env, var, shell->error_code);
	if (!expand_var)
		return (NULL);
	total_len += ft_strlen(ms_get_env_value(shell->env_data->env, var, shell->error_code));
	total_len = ft_strlen(arg) + ft_strlen(expand_var) - ft_strlen(var);
	new_arg = (char *)gc_malloc(sizeof(char) * (total_len), shell->gcl);
	copy_expanded_cmd(new_arg, arg, expand_var, is_nested);
	if (is_var(new_arg))
		new_arg = nested_vars(new_arg, shell);
	return (new_arg);
}
