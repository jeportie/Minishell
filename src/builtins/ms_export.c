/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:23:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/12 14:43:30 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

static char	*st_extract_value(t_export_utils *utils, char *cmd, int size,
		char *folder)
{
	int (start) = 0;
	char *(value) = NULL;
	int (i) = 0;
	if (utils->flag >= 1)
	{
		size += 1 + utils->flag;
		start = size;
		while (cmd[size++])
			i++;
	}
	else
		return (NULL);
	value = gc_malloc((i + 1) * sizeof(char), utils->shell->gcl);
	if (!value)
		echec_malloc(utils->shell->gcl, "value");
	gc_lock(value, utils->shell->gcl);
	value[i] = '\0';
	i = 0;
	while (cmd[start])
		value[i++] = cmd[start++];
	return (value);
}

static void	st_order_env(t_exec_context *context)
{
	t_env *(current) = context->shell->env_data->env;
	t_env *(sorted) = NULL;
	t_env *(next) = NULL;
	t_env *(temp) = NULL;
	while (current)
	{
		next = current->next;
		if (!sorted || ft_strncmp(current->var,
				sorted->var, ft_strlen(sorted->var)) < 0)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			temp = sorted;
			while (temp->next && ft_strncmp(temp->next->var,
					current->var, ft_strlen(current->var)) < 0)
				temp = temp->next;
			current->next = temp->next;
			temp->next = current;
		}
		current = next;
	}
	context->shell->env_data->env = sorted;
}

void	st_print_order(t_exec_context *context)
{
	t_env	*tmp;

	tmp = context->shell->env_data->env;
	while (tmp)
	{
		if (ft_strncmp(tmp->var, "_", 2))
		{
			ft_dprintf(1, "export ", tmp->var);
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
	utils->flag = 1;
	utils->var = NULL;
	utils->value = NULL;
}

int	ms_export(t_cmd_node *cmd_node, t_exec_context *context)
{
	int (i) = 1;
	t_export_utils (utils);
	st_init_utils(&utils, context->shell);
	if (cmd_node->argc == 1)
	{
		st_order_env(context);
		st_print_order(context);
		return (0);
	}
	while (cmd_node->argv[i])
	{
		utils.var = extract_folder(&utils, cmd_node->argv[i]);
		if (!utils.var)
			return (1);
		utils.value = st_extract_value(&utils, cmd_node->argv[i],
				ft_strlen(utils.var) - 1, utils.var);
		add_export(&utils, context->shell->env_data->env, utils.var,
			utils.value);
		gc_unlock(utils.var, context->shell->gcl);
		if (utils.value)
			gc_unlock(utils.value, context->shell->gcl);
		i++;
	}
	gc_collect(context->shell->gcl);
	return (0);
}

/*int	ft_export(t_infos *infos, t_tok *tmp)
{
	char	*value;
	char	*folder;

	int (i) = 1;
	if (!tmp->cmd[1])
	{
		ft_order_env(infos);
		ft_print_order(infos);
		return (0);
	}
	while (tmp->cmd[i])
	{
		folder = ft_extract_folder(infos, tmp->cmd[i]);
		if (!folder)
			return (1);
		value = ft_extract_value(infos, tmp->cmd[i], ft_strlen(folder) - 1,
				folder);
		add_export(infos, infos->tmp_env, folder, value);
		folder = ft_free_str(folder);
		if (value)
			value = ft_free_str(value);
		i++;
	}
	return (0);
}*/
