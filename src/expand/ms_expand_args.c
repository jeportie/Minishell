/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:04:44 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/08 15:35:43 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/expand.h"
#include "../../include/minishell.h"

static int	get_prefix_len(char *arg, bool is_nested)
{
	char	*dollar_ptr;

	if (is_nested)
		return (0);
	dollar_ptr = ft_strchr(arg, '$');
	if (dollar_ptr)
		return (dollar_ptr - arg);
	return (0);
}

static int	get_var_len_with_braces(t_expand_utils *ex_utils)
{
	int	len;

	len = ft_strlen(ex_utils->var) + 1;
	if (ex_utils->start_var && ex_utils->start_var[-1] == '{')
		len++;
	if (ex_utils->start_var
		&& ex_utils->start_var[ft_strlen(ex_utils->var)] == '}')
		len++;
	return (len);
}

static void	copy_parts(char *dest, char *arg, t_expand_utils *ex_utils,
		int prefix_len)
{
	int	expanded_len;
	int	suffix_index;

	expanded_len = ft_strlen(ex_utils->expand_var);
	ft_memcpy(dest, arg, prefix_len);
	ft_memcpy(dest + prefix_len, ex_utils->expand_var, expanded_len);
	suffix_index = prefix_len + get_var_len_with_braces(ex_utils);
	ft_strlcpy(dest + prefix_len + expanded_len, arg + suffix_index,
		ex_utils->total_len);
}

void	assemble_expanded_arg(char *arg, bool is_nested,
			t_expand_utils *ex_utils)
{
	int	prefix_len;

	prefix_len = get_prefix_len(arg, is_nested);
	copy_parts(ex_utils->new_arg, arg, ex_utils, prefix_len);
	ex_utils->new_arg[ex_utils->total_len] = '\0';
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
	assemble_expanded_arg(arg, is_nested, &ex_utils);
	if (is_var(ex_utils.new_arg))
		ex_utils.new_arg = nested_vars(ex_utils.new_arg, shell);
	return (ex_utils.new_arg);
}
