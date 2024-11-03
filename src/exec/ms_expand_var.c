/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 17:30:06 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/03 17:33:58 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/minishell.h"
#include <ctype.h>

static char	*handle_dollar_at_end(char *result)
{
	return (ft_strjoin_free(result, ft_strdup("$")));
}

static char	*handle_exit_status(char *result, t_env_data *env_data)
{
	char	*exit_status;

	exit_status = ft_itoa(env_data->last_exit_status);
	return (ft_strjoin_free(result, exit_status));
}

static char	*handle_braced_variable(const char *token, size_t *i, t_env_data *env_data, char *result)
{
	size_t	start;
	char	*var_name;
	char	*var_value;

	start = ++(*i);
	while (token[*i] && token[*i] != '}')
		(*i)++;
	if (token[*i] == '}')
	{
		var_name = ft_substr(token, start, *i - start);
		var_value = ms_getenv(var_name, env_data);
		result = ft_strjoin_free(result, ft_strdup(var_value ? var_value : ""));
		free(var_name);
		(*i)++;
	}
	else
	{
		ms_handle_error("Minishell: unmatched '{'\n", NULL);
		free(result);
		return (NULL);
	}
	return (result);
}

static char	*handle_variable(const char *token, size_t *i, t_env_data *env_data, char *result)
{
	size_t	start;
	char	*var_name;
	char	*var_value;

	start = *i;
	while (token[*i] && (isalnum(token[*i]) || token[*i] == '_'))
		(*i)++;
	var_name = ft_substr(token, start, *i - start);
	var_value = ms_getenv(var_name, env_data);
	result = ft_strjoin_free(result, ft_strdup(var_value ? var_value : ""));
	free(var_name);
	return (result);
}

static char	*handle_regular_characters(const char *token, size_t *i, char *result)
{
	size_t	start;
	char	*text;

	start = *i;
	while (token[*i] && token[*i] != '$')
		(*i)++;
	text = ft_substr(token, start, *i - start);
	result = ft_strjoin_free(result, text);
	return (result);
}

char	*ms_expand_variables(const char *token, t_env_data *env_data)
{
	char	*result;
	size_t	i;

	result = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '$')
		{
			i++;
			if (token[i] == '\0')
			{
				result = handle_dollar_at_end(result);
				break ;
			}
			else if (token[i] == '?')
				result = handle_exit_status(result, env_data);
			else if (token[i] == '{')
				result = handle_braced_variable(token, &i, env_data, result);
			else
				result = handle_variable(token, &i, env_data, result);
			if (!result)
				return (NULL);
		}
		else
			result = handle_regular_characters(token, &i, result);
	}
	return (result);
}
