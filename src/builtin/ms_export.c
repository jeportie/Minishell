/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:23:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 18:00:01 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

/*void	ft_add_cat(t_env *current, t_env *tmp, int flag)
{
	if (flag == 0)
	{
		current->next->value_folder
			= ft_strjoin(current->next->value_folder,
				tmp->value_folder, '\0', 2);
		tmp->name_folder = ft_free_str(tmp->name_folder);
		free(tmp);
	}
	else
	{
		current->next->value_folder
			= ft_free_str(current->next->value_folder);
		current->next->value_folder = tmp->value_folder;
		tmp->name_folder = ft_free_str(tmp->name_folder);
		free(tmp);
	}
}

void	ft_add_export_utils(t_infos *infos, t_env *current, t_env *tmp)
{
	int (i) = 0;
	while (current->next)
	{
		if (!ft_strncmp(current->next->name_folder, tmp->name_folder,
				ft_strlen(tmp->name_folder) + 1))
		{
			i = 1;
			if (tmp->value_folder)
			{
				if (infos->flag == 2)
					ft_add_cat(current, tmp, 0);
				else
					ft_add_cat(current, tmp, 1);
				break ;
			}
			else
			{
				tmp->name_folder = ft_free_str(tmp->name_folder);
				free(tmp);
			}
		}
		current = current->next;
	}
	if (i != 1)
		current->next = tmp;
}

void	add_export(t_infos *infos, t_env *ev, char *name_folder,
	char *value_folder)
{
	t_env *(current) = ev;
	t_env *(tmp) = malloc(sizeof(t_env));
	if (!tmp)
		ft_quit(infos, 2, "Error: echec malloc tmp.\n", errno);
	tmp->name_folder = ft_strdup(name_folder);
	if (!tmp->name_folder)
		ft_quit(infos, 2, "Error: echec malloc tmp->name_folder.\n", errno);
	if (value_folder)
	{
		tmp->value_folder = ft_strdup(value_folder);
		if (!tmp->value_folder)
			ft_quit(infos, 2, "Error: echec malloc tmp->value_folder.\n",
				errno);
	}
	else
		tmp->value_folder = NULL;
	tmp->next = NULL;
	if (current)
		ft_add_export_utils(infos, current, tmp);
	else
		ev = tmp;
}*/

static int	ft_valide_var(char *input)
{
	int	i;

	i = 0;
	if (ft_isdigit(input[i]))
	{
		ft_dprintf(2, "minishell: export: `%s\':"
			" not a valid identifier\n", input);
		return (0);
	}
	while (input[i])
	{
		if (ft_isalnum(input[i]) || input[i] == '_')
			i++;
		else if ((input[i] == '=') || (input[i] == '+' && input[i + 1] == '='))
			return (i);
		else
		{
			ft_dprintf(2, "minishell: export: `%s\':"
				" not a valid identifier\n", input);
			return (0);
		}
	}
	return (i);
}

static char	*ft_extract_folder(t_shell *shell, char *cmd)
{
	char	*folder;

	int (size) = ft_valide_var(cmd);
	int (i) = 0;
	if (size == 0)
		return (NULL);
	if (cmd[size] == '+' && cmd[size + 1] == '=')
		infos->flag = 2;
	else if (cmd[size] == '=')
		infos->flag = 1;
	else
		infos->flag = 0;
	folder = malloc(size + 1);
	if (!folder)
	{
		gc_cleanup(shell->gcl);
		ft_dprintf(2, "Error: echec malloc folder\n");
		exit(1);
	}
	folder[size] = '\0';
	while (i < size)
	{
		folder[i] = cmd[i];
		i++;
	}
	return (folder);
}

static char	*ft_extract_value(t_shell *shell, char *cmd, int size, char *folder)
{
	int (start) = 0;
	char *(value) = NULL;
	int (i) = 0;
	if (infos->flag >= 1)
	{
		size += 1 + infos->flag;
		start = size;
		while (cmd[size++])
			i++;
	}
	else
		return (NULL);
	value = malloc(i + 1);
	if (!value)
	{
		free(folder);
		gc_cleanup(shell->gcl);
		ft_dprintf(2, "Error: echec malloc value\n");
		exit(1);
	}
	value[i] = '\0';
	i = 0;
	while (cmd[start])
		value[i++] = cmd[start++];
	return (value);
}

void	ft_order_env(t_exec_context *context)
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

void	ft_print_order(t_exec_context *context)
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

typedef struct s_export_utils
{
	t_shell	*shell;
	int		flag;
	char	*var;
	char	*value;
}				t_export_utils;

static void	st_init_utils(t_export_utils *utils, t_shell *shell)
{
	utils->shell = shell;
	utils->flag = 1;
	utils->var = NULL;
	utils->value = NULL;
}

int	ms_export(t_cmd_node *cmd_node, t_exec_context *context)
{
	t_export_utils	utils;

	int (i) = 1;
	st_init_utils(&utils, context->shell);
	if (cmd_node->argc == 1)
	{
		ft_order_env(context);
		ft_print_order(context);
		return (0);
	}
	while (cmd_node->argv[i])
	{
		utils.var = ft_extract_folder(context->shell, cmd_node->argv[i]);
		if (!utils.var)
			return (1);
		utils.value = ft_extract_value(context->shell, cmd_node->argv[i],
				ft_strlen(utils.var) - 1, utils.var);
		add_export(context->shell, context->shell->env_data->env, utils.var, utils.value);
		free(utils.var);
		if (utils.value)
			free(utils.value);
		i++;
	}
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
