/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:23:38 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 14:00:11 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static void	st_order_env(t_env **tmp_env)
{
	t_env *(current) = *tmp_env;
	t_env *(sorted) = NULL;
	t_env *(next) = NULL;
	t_env *(temp) = NULL;
	while (current)
	{
		next = current->next;
		if (!sorted || ft_strncmp(current->var, sorted->var,
				ft_strlen(current->var) + 1) < 0)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			temp = sorted;
			while (temp->next && ft_strncmp(temp->next->var, current->var,
					ft_strlen(current->var) + 1) < 0)
				temp = temp->next;
			current->next = temp->next;
			temp->next = current;
		}
		current = next;
	}
	*tmp_env = sorted;
}

static void	st_print_order(t_env *tmp_env, int fd)
{
	t_env	*tmp;

	tmp = tmp_env;
	while (tmp)
	{
		if (ft_strncmp(tmp->var, "_", 2))
		{
			ms_safe_putstr_fd("export ", fd);
			if (tmp->var)
				ms_safe_putstr_fd(tmp->var, fd);
			if (tmp->value)
			{
				ms_safe_putstr_fd("=\"", fd);
				ms_safe_putstr_fd(tmp->value, fd);
				ms_safe_putstr_fd("\"", fd);
			}
			ms_safe_putstr_fd("\n", fd);
		}
		tmp = tmp->next;
	}
}

static void	st_init_utils(t_export_utils *utils, t_shell *shell)
{
	utils->shell = shell;
	utils->flag = 0;
	utils->var = NULL;
	utils->value = NULL;
}

int	ms_export(t_cmd_node *cmd_node, t_exec_context *context)
{
	t_export_utils	utils;

	t_env *(tmp_env) = ms_copy_env_list(context->shell->env_data->env,
			context->shell->gcl);
	int (i) = 1;
	st_init_utils(&utils, context->shell);
	if (cmd_node->argc == 1)
		return (st_order_env(&tmp_env), st_print_order(tmp_env,
				context->stdout_fd), 0);
	while (cmd_node->argv[i])
	{
		utils.flag = 0;
		utils.var = ms_extract_folder(&utils, cmd_node->argv[i]);
		if (!utils.var)
			return (1);
		utils.value = ms_extract_value(&utils, cmd_node->argv[i],
				ft_strlen(utils.var) - 1);
		ms_add_export(&utils, &context->shell->env_data->env, utils.var,
			utils.value);
		i++;
	}
	return (0);
}
