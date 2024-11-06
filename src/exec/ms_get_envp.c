/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 14:28:24 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 11:58:49 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/exec.h"

static int	count_envlines(t_env *envi)
{
	int		i;
	t_env	*current;

	i = 0;
	current = envi;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

void	malloc_error(t_gc *gcl)
{
	ft_dprintf(STDERR_FILENO, "minishell: error: malloc failed.\n");
	gc_cleanup(gcl);
	exit(EXIT_FAILURE);
}

void	parse_env(char **result, int *i, t_env *env, t_gc *gcl)
{
	int		line_len;
	int		var_len;
	int		value_len;
	char	*dest_ptr;

	var_len = ft_strlen(env->var);
	value_len = ft_strlen(env->value);
	line_len = var_len + 1 + value_len + 1;
	result[*i] = gc_malloc(sizeof(char) * line_len, gcl);
	if (!result[*i])
		malloc_error(gcl);
	dest_ptr = result[*i];
	ft_memcpy(dest_ptr, env->var, var_len);
	dest_ptr += var_len;
	*dest_ptr = '=';
	dest_ptr++;
	ft_memcpy(dest_ptr, env->value, value_len);
	dest_ptr += value_len;
	*dest_ptr = '\0';
}

char	**ms_get_envp(t_env *env, t_gc *gcl)
{
	int		env_lines;
	char	**result;
	int		i;

	env_lines = count_envlines(env);
	result = (char **)gc_malloc(sizeof(char *) * (env_lines + 1), gcl);
	if (!result)
		malloc_error(gcl);
	i = 0;
	while (env)
	{
		if (!env->var)
			env->var = "";
		if (!env->value)
			env->value = "";
		parse_env(result, &i, env, gcl);
		i++;
		env = env->next;
	}
	result[i] = NULL;
	return (result);
}
