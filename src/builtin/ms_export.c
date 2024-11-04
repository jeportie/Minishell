/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:23:38 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/04 13:24:08 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

/*static char	*ft_extract_folder(t_infos *infos, char *cmd)
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
		ft_quit(infos, 2, "Error: echec malloc folder\n ", errno);
	folder[size] = '\0';
	while (i < size)
	{
		folder[i] = cmd[i];
		i++;
	}
	return (folder);
}

static char	*ft_extract_value(t_infos *infos, char *cmd, int size, char *folder)
{
	int		start;
	char	*value;

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
		folder = ft_free_str(folder);
		ft_quit(infos, 2, "Error: echec malloc value\n", errno);
	}
	value[i] = '\0';
	i = 0;
	while (cmd[start])
		value[i++] = cmd[start++];
	return (value);
}

void	ft_order_env(t_infos *infos)
{
	t_env *(current) = infos->tmp_env;
	t_env *(sorted) = NULL;
	t_env *(next) = NULL;
	t_env *(temp) = NULL;
	while (current)
	{
		next = current->next;
		if (!sorted || ft_strncmp(current->name_folder,
				sorted->name_folder, ft_strlen(sorted->name_folder)) < 0)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			temp = sorted;
			while (temp->next && ft_strncmp(temp->next->name_folder,
					current->name_folder, ft_strlen(current->name_folder)) < 0)
				temp = temp->next;
			current->next = temp->next;
			temp->next = current;
		}
		current = next;
	}
	infos->tmp_env = sorted;
}

void	ft_print_order(t_infos *infos)
{
	t_env	*tmp;

	tmp = infos->tmp_env;
	while (tmp)
	{
		if (ft_strncmp(tmp->name_folder, "_", 2))
		{
			ft_dprintf(1, "export ", tmp->name_folder, NULL);
			if (tmp->value_folder)
				ft_dprintf(1, "=\"", tmp->value_folder, "\"");
			ft_dprintf(1, "\n", NULL, NULL);
		}
		tmp = tmp->next;
	}
}

int	ft_export(t_infos *infos, t_tok *tmp)
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
