/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 21:35:57 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/20 15:23:31 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

void	add_cat(t_shell *shell, t_env *current, t_env *tmp, int flag)
{
	char	*tmp_str;

	if (flag == 0)
	{
		tmp_str = current->value;
		current->value = ft_strjoin(current->value, tmp->value);
		gc_register(current->value, shell->gcl);
		gc_lock(current->value, shell->gcl);
		gc_free(tmp_str, shell->gcl);
	}
	else
	{
		gc_free(current->value, shell->gcl);
		current->value = tmp->value;
		tmp->value = NULL;
	}
	gc_free(tmp->var, shell->gcl);
	gc_free(tmp, shell->gcl);
}

void	add_export_utils(t_export_utils *utils, t_env **env, t_env *tmp)
{
	t_env *(current) = *env;
	t_env *(prev) = NULL;
	int (found) = 0;
	while (current)
	{
		if (!ft_strncmp(current->var, tmp->var, ft_strlen(tmp->var) + 1))
		{
			if (tmp->value && utils->flag == 2)
				add_cat(utils->shell, current, tmp, 0);
			else
				add_cat(utils->shell, current, tmp, 1);
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

void	add_export(t_export_utils *utils, t_env **ev, char *name_folder,
		char *value_folder)
{
	t_env *(current) = *ev;
	t_env *(tmp) = gc_malloc(sizeof(t_env), utils->shell->gcl);
	if (!tmp)
		echec_malloc(utils->shell->gcl, "tmp");
	gc_lock(tmp, utils->shell->gcl);
	tmp->var = gc_strdup(name_folder, utils->shell->gcl);
	if (!tmp->var)
		echec_malloc(utils->shell->gcl, "tmp->var");
	if (value_folder)
	{
		tmp->value = gc_strdup(value_folder, utils->shell->gcl);
		if (!tmp->value)
			echec_malloc(utils->shell->gcl, "tmp->value");
	}
	else
		tmp->value = NULL;
	tmp->next = NULL;
	if (current)
		add_export_utils(utils, ev, tmp);
	else
		*ev = tmp;
}

char	*extract_value(t_export_utils *utils, char *cmd, int size)
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

char	*extract_folder(t_export_utils *utils, char *cmd)
{
	char *(folder) = NULL;
	int (size) = valide_var(cmd);
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
	gc_lock(folder, utils->shell->gcl);
	if (!folder)
		echec_malloc(utils->shell->gcl, "folder");
	folder[size] = '\0';
	while (i < size)
	{
		folder[i] = cmd[i];
		i++;
	}
	return (folder);
}
