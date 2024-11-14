/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:04:44 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/14 16:09:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/env_value.h"

static int	var_len(char *arg, t_gc *gcl)
{
	int		i;

	i = 0;
	int (len) = 0;
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

static char	*find_dollar(char *arg)
{
	int		i;

	if (!arg)
		return (NULL);
	i = 0;
	while (arg[i] && arg[i] != '$')
		i++;
	if (arg[i] != '$')
		return (NULL);
	printf("$ is %i away from first char.\n", i);
	i++;
	return (&arg[i]);
}

static void	copy_expanded_cmd(char *new_arg, char *arg, char *expand_var,
			t_gc *gcl)
{
	int (i) = 0;
	int (j) = 0;
	while (arg[i] && arg[i] != '$')
	{
		new_arg[i] = arg[i];
		i++;
	}
	j = 0;
	int (h) = i + 1;
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
		if (arg[h] == '{')
		{
			while (arg[h] != '}')
				h++;
		}
		new_arg[i] = arg[++h];
		i++;
		h++;
	}
	new_arg[i] = '\0';
	gc_free(arg, gcl);
}

char	*ms_expand_arg(char *arg, t_env *env, t_gc *gcl)
{
	char	*new_arg;
	char	*var;
	char	*expand_var;
	char	*start_var;
	int		total_len;

	total_len = 0;
	start_var = find_dollar(arg);
	var = ms_extract_var(start_var, gcl);
	expand_var = ms_get_env_value(env, var);
	total_len += ft_strlen(ms_get_env_value(env, var));
	total_len = ft_strlen(arg) + ft_strlen(expand_var) - ft_strlen(var);
	new_arg = (char *)gc_malloc(sizeof(char) * (total_len), gcl);
	copy_expanded_cmd(new_arg, arg, expand_var, gcl);
	if (is_var(new_arg))
		new_arg = ms_expand_arg(new_arg, env, gcl);
	return (new_arg);
}
