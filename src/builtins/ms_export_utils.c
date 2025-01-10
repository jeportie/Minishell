/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarquis <gmarquis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 21:35:57 by gmarquis          #+#    #+#             */
/*   Updated: 2025/01/10 14:00:06 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

void	ms_add_cat(t_shell *shell, t_env *current, t_env *tmp, int flag)
{
	if (flag == 0)
	{
		current->value = ft_strjoin(current->value, tmp->value);
		gc_register(current->value, shell->gcl);
		gc_lock(current->value, shell->gcl);
	}
	else
	{
		current->value = tmp->value;
		tmp->value = NULL;
	}
}

void	ms_add_export_utils(t_export_utils *utils, t_env **env, t_env *tmp)
{
	t_env *(current) = *env;
	t_env *(prev) = NULL;
	int (found) = 0;
	while (current)
	{
		if (!ft_strncmp(current->var, tmp->var, ft_strlen(tmp->var) + 1))
		{
			if (tmp->value && utils->flag == 2)
				ms_add_cat(utils->shell, current, tmp, 0);
			else
				ms_add_cat(utils->shell, current, tmp, 1);
			found = 1;
			break ;
		}
		prev = current;
		current = current->next;
	}
	if (!found)
	{
		if (prev == NULL)
			*env = tmp;
		else
			prev->next = tmp;
	}
}

void	ms_add_export(t_export_utils *utils, t_env **ev, char *name_folder,
		char *value_folder)
{
	t_env *(current) = *ev;
	t_env *(tmp) = gc_malloc(sizeof(t_env), utils->shell->gcl);
	if (!tmp)
		ms_echec_malloc(utils->shell->gcl, "tmp");
	gc_lock(tmp, utils->shell->gcl);
	tmp->var = gc_strdup(name_folder, utils->shell->gcl);
	if (!tmp->var)
		ms_echec_malloc(utils->shell->gcl, "tmp->var");
	gc_lock(tmp->var, utils->shell->gcl);
	if (value_folder)
	{
		tmp->value = gc_strdup(value_folder, utils->shell->gcl);
		if (!tmp->value)
			ms_echec_malloc(utils->shell->gcl, "tmp->value");
		gc_lock(tmp->value, utils->shell->gcl);
	}
	else
		tmp->value = NULL;
	tmp->next = NULL;
	if (current)
		ms_add_export_utils(utils, ev, tmp);
	else
		*ev = tmp;
}

char	*ms_extract_value(t_export_utils *utils, char *cmd, int size)
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
		ms_echec_malloc(utils->shell->gcl, "value");
	gc_lock(value, utils->shell->gcl);
	value[i] = '\0';
	i = 0;
	while (cmd[start])
		value[i++] = cmd[start++];
	return (value);
}

char	*ms_extract_folder(t_export_utils *utils, char *cmd)
{
	char *(folder) = NULL;
	int (size) = ms_valide_var(cmd);
	int (i) = 0;
	if (size == 0)
		return (NULL);
	if (cmd[size])
	{
		if (cmd[size + 1] && cmd[size] == '+' && cmd[size + 1] == '=')
			utils->flag = 2;
		else if (cmd[size] == '=')
			utils->flag = 1;
		else
			utils->flag = 0;
	}
	folder = gc_malloc((size + 1) * sizeof(char), utils->shell->gcl);
	if (!folder)
		ms_echec_malloc(utils->shell->gcl, "folder");
	gc_lock(folder, utils->shell->gcl);
	folder[size] = '\0';
	while (i < size)
	{
		folder[i] = cmd[i];
		i++;
	}
	return (folder);
}
