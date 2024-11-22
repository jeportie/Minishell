/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:23:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/22 16:27:31 by jeportie         ###   ########.fr       */
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

static void	st_print_order(t_env *tmp_env, t_env_data *env_data)
{
	t_env	*tmp;

	tmp = tmp_env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->var, "_", 2) && env_data->oldpwd == 1)
		{
			ft_dprintf(1, "export OLDPWD\n");
			return ;
		}
		else if (ft_strncmp(tmp->var, "_", 2))
		{
			ft_dprintf(1, "export ");
			if (tmp->var)
				ft_dprintf(1, "%s", tmp->var);
			if (tmp->value)
				ft_dprintf(1, "=\"%s\"", tmp->value);
			ft_dprintf(1, "\n");
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

	t_env *(tmp_env) = copy_env_list(context->shell->env_data->env,
			context->shell->gcl);
	int (i) = 1;
	st_init_utils(&utils, context->shell);
	if (cmd_node->argc == 1)
		return (st_order_env(&tmp_env), st_print_order(tmp_env,
				context->shell->env_data), 0);
	while (cmd_node->argv[i])
	{
		utils.flag = 0;
		utils.var = extract_folder(&utils, cmd_node->argv[i]);
		if (!utils.var)
			return (1);
		utils.value = extract_value(&utils, cmd_node->argv[i],
				ft_strlen(utils.var) - 1);
		add_export(&utils, &context->shell->env_data->env, utils.var,
			utils.value);
		i++;
	}
	return (0);
}
