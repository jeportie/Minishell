/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 21:35:57 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 21:46:45 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

void	add_cat(t_shell *shell, t_env *current, t_env *tmp, int flag)
{
	char	*tmp_str;

	if (flag == 0)
	{
		tmp_str = current->next->value;
		current->next->value
			= ft_strjoin(current->next->value,
				tmp->value);
		gc_unlock(tmp_str, shell->gcl);
		gc_unlock(tmp->value, shell->gcl);
		gc_unlock(tmp->var, shell->gcl);
		gc_unlock(tmp, shell->gcl);
	}
	else
	{
		gc_unlock(current->next->value, shell->gcl);
		current->next->value = tmp->value;
		gc_unlock(tmp->var, shell->gcl);
		gc_unlock(tmp, shell->gcl);
	}
}

void	add_export_utils(t_export_utils *utils, t_env *current, t_env *tmp)
{
	int (i) = 0;
	while (current->next)
	{
		if (!ft_strncmp(current->next->var, tmp->var,
				ft_strlen(tmp->var) + 1))
		{
			i = 1;
			if (tmp->value)
			{
				if (utils->flag == 2)
					add_cat(utils->shell, current, tmp, 0);
				else
					add_cat(utils->shell, current, tmp, 1);
				break ;
			}
			else
			{
				gc_unlock(tmp->var, utils->shell->gcl);
				gc_unlock(tmp, utils->shell->gcl);
			}
		}
		current = current->next;
	}
	if (i != 1)
		current->next = tmp;
}

void	add_export(t_export_utils *utils, t_env *ev, char *name_folder,
		char *value_folder)
{
	t_env *(current) = ev;
	t_env *(tmp) = gc_malloc(sizeof(t_env), utils->shell->gcl);
	if (!tmp)
		echec_malloc(utils->shell->gcl, "tmp");
	gc_lock(tmp, utils->shell->gcl);
	tmp->var = ft_strdup(name_folder);
	if (!tmp->var)
		echec_malloc(utils->shell->gcl, "tmp->var");
	gc_register(tmp->var, utils->shell->gcl);
	gc_lock(tmp->var, utils->shell->gcl);
	if (value_folder)
	{
		tmp->value = ft_strdup(value_folder);
		if (!tmp->value)
			echec_malloc(utils->shell->gcl, "tmp->value");
		gc_register(tmp->value, utils->shell->gcl);
		gc_lock(tmp->value, utils->shell->gcl);
	}
	else
		tmp->value = NULL;
	tmp->next = NULL;
	if (current)
		add_export_utils(utils, current, tmp);
	else
		ev = tmp;
}

int	valide_var(char *input)
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

char	*extract_folder(t_export_utils *utils, char *cmd)
{
	char *(folder) = NULL;
	int (size) = valide_var(cmd);
	int (i) = 0;
	if (size == 0)
		return (NULL);
	if (cmd[size] == '+' && cmd[size + 1] == '=')
		utils->flag = 2;
	else if (cmd[size] == '=')
		utils->flag = 1;
	else
		utils->flag = 0;
	folder = gc_malloc(sizeof(size + 1), utils->shell->gcl);
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
