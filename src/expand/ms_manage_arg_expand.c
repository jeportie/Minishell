/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_manage_arg_expand.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:12:43 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/10 13:19:41 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/expand.h"

int	ms_var_len(char *arg, t_gc *gcl)
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

char	*ms_extract_var(char *arg, t_gc *gcl)
{
	int (i) = 0;
	int (j) = 0;
	int (len) = ms_var_len(arg, gcl);
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

static	void	st_expand_argv(char **new_argv, char **argv,
		t_wildcard_context *match_ctx)
{
	int		argv_index;
	int		new_argv_index;
	int		match_index;

	argv_index = 0;
	new_argv_index = 0;
	match_index = 0;
	while (argv[argv_index])
	{
		if (ft_strchr(argv[argv_index], '*'))
		{
			while (match_index < (int)match_ctx->match_count)
			{
				new_argv[new_argv_index] = match_ctx->matches[match_index];
				new_argv_index++;
				match_index++;
			}
		}
		else
			new_argv[new_argv_index] = argv[argv_index];
		argv_index++;
		new_argv_index++;
	}
}

char	**ms_cmd_arg_expansion(char **argv, t_wildcard_context *match_ctx,
			t_gc *gcl)
{
	char	**new_argv;
	int		i;
	int		total_args;

	i = 0;
	total_args = (int)match_ctx->match_count;
	while (argv[i])
	{
		i++;
		total_args++;
	}
	new_argv = (char **)gc_malloc(sizeof(char *) * total_args, gcl);
	ft_memset(new_argv, 0, sizeof(char *) * total_args);
	st_expand_argv(new_argv, argv, match_ctx);
	return (new_argv);
}

void	ms_manage_arg_expand(t_cmd_node *cmd_node,
			t_exec_context *context, t_gc *gcl)
{
	t_wildcard_context *(matches) = NULL;
	int (i) = 0;
	if (!ms_is_equal(cmd_node->argv[0]))
	{
		while (i < cmd_node->argc)
		{
			if (ms_is_var(cmd_node->argv[i]) && cmd_node->is_expand == false)
			{
				cmd_node->argv[i] = ms_nested_vars(cmd_node->argv[i],
						context->shell);
			}
			if (ms_is_wild(cmd_node->argv[i]))
			{
				matches = ms_expand_wild(cmd_node->argv[i], gcl);
				if (matches->matches)
				{
					cmd_node->argc += matches->match_count - 1;
					cmd_node->argv = ms_cmd_arg_expansion(cmd_node->argv,
							matches, gcl);
				}
			}
			i++;
		}
	}
}
