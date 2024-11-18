/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_args_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:46:34 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/18 17:48:59 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

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

char	*copy_argv_to_str(char *dest, int argc, char **argv)
{
	int		result_index;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result_index = 0;
	while (i < argc && argv[i])
	{
		while (argv[i][j])
		{
			dest[result_index] = argv[i][j];
			result_index++;
			j++;
		}
		j = 0;
		i++;
	}
	dest[result_index] = '\0';
	return (dest);
}

char	*ft_strsjoin(int argc, char **argv, t_gc *gcl)
{
	char	*result;
	int		i;
	int		len;

	if (!argv || !*argv)
		return (NULL);
	i = 0;
	len = 0;
	while (i < argc)
	{
		if (argv[i])
			len += ft_strlen(argv[i]);
		i++;
	}
	result = gc_malloc(sizeof(char) * (len + 1), gcl);
	gc_lock(result, gcl);
	copy_argv_to_str(result, argc, argv);
	return (result);
}

char	**expand_nested_vars(char *arg, int dollars, t_env *env, t_gc *gcl)
{
	char	**expanded_vars;
	int		i;

	char **(vars) = ft_split(arg, '$');
	gc_nest_register(vars, gcl);
	gc_lock(vars, gcl);
	if (arg[0] != '$')
		dollars++;
	expanded_vars = gc_malloc(sizeof(char *) * (dollars + 1), gcl);
	i = 0;
	while (i < dollars)
	{
		if (i == 0 && arg[i] != '$')
		{
			expanded_vars[i] = vars[i];
			i++;
		}
		else
		{
			expanded_vars[i] = ms_expand_arg(vars[i], env, true, gcl);
			i++;
		}
	}
	expanded_vars[i] = NULL;
	return (expanded_vars);
}

char	*nested_vars(char *arg, t_env *env, t_gc *gcl)
{
	char	*result;
	char	**expanded_vars;

	int (dollars) = count_dollars(arg);
	if (dollars < 2)
		return (ms_expand_arg(arg, env, false, gcl));
	else
		expanded_vars = expand_nested_vars(arg, dollars, env, gcl);
	if (is_equal(expanded_vars[0]))
		result = ft_strsjoin(dollars, &expanded_vars[1], gcl);
	else
		result = ft_strsjoin(dollars, expanded_vars, gcl);
	return (result);
}
