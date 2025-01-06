/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fix_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:36:14 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/04 13:50:33 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*st_get_path(t_env_data *env_data)
{
	t_env	*current;
	int		trigger;

	trigger = 0;
	current = env_data->env;
	while (current)
	{
		if (current->var && !ft_strncmp(current->var, "PATH", 5))
		{
			trigger = 1;
			return (current->value);
			break ;
		}
		current = current->next;
	}
	if (trigger == 0)
		return ("/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	return (NULL);
}

static char	*st_get_term(t_env_data *env_data)
{
	t_env	*current;
	int		trigger;

	trigger = 0;
	current = env_data->env;
	while (current)
	{
		if (current->var && !ft_strncmp(current->var, "TERM", 5))
		{
			trigger = 1;
			return (current->value);
			break ;
		}
		current = current->next;
	}
	if (trigger == 0)
		return ("dumb");
	return (NULL);
}

static char	*st_get_shell(t_env_data *env_data)
{
	t_env	*current;
	int		trigger;

	trigger = 0;
	current = env_data->env;
	while (current)
	{
		if (current->var && !ft_strncmp(current->var, "SHELL", 5))
		{
			trigger = 1;
			return (current->value);
			break ;
		}
		current = current->next;
	}
	if (trigger == 0)
		return ("/bin/zsh");
	return (NULL);
}

static char	*st_get_user(t_env_data *env_data)
{
	t_env	*current;
	int		trigger;

	trigger = 0;
	current = env_data->env;
	while (current)
	{
		if (current->var && !ft_strncmp(current->var, "USER", 5))
		{
			trigger = 1;
			return (current->value);
			break ;
		}
		current = current->next;
	}
	if (trigger == 0)
		return ("dead");
	return (NULL);
}

void	init_fix_value(t_env_data *env_data)
{
	env_data->path = st_get_path(env_data);
	env_data->term = st_get_term(env_data);
	env_data->shell = st_get_shell(env_data);
	env_data->user = st_get_user(env_data);
}
