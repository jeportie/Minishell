/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 21:35:57 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/05 10:16:36 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"

void	add_cat(t_env *current, t_env *tmp, int flag)
{
	char	*tmp_str;

	if (flag == 0)
	{
		tmp_str = current->next->value;
		current->next->value
			= ft_strjoin(current->next->value,
				tmp->value);
		free(tmp_str);
		free(tmp->value);
		free(tmp->var);
		free(tmp);
	}
	else
	{
		free(current->next->value);
		current->next->value = tmp->value;
		free(tmp->var);
		free(tmp);
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
					add_cat(current, tmp, 0);
				else
					add_cat(current, tmp, 1);
				break ;
			}
			else
			{
				free(tmp->var);
				free(tmp);
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
	t_env *(tmp) = malloc(sizeof(t_env));
	if (!tmp)
	{
		gc_cleanup(utils->shell->gcl);
		ft_dprintf(2, "Error: echec malloc tmp.\n");
		exit (1);
	}
	tmp->var = ft_strdup(name_folder);
	if (!tmp->var)
	{
		gc_cleanup(utils->shell->gcl);
		ft_dprintf(2, "Error: echec malloc tmp->name_folder.\n");
		exit (1);
	}
	if (value_folder)
	{
		tmp->value = ft_strdup(value_folder);
		if (!tmp->value)
		{
			gc_cleanup(utils->shell->gcl);
			ft_dprintf(2, "Error: echec malloc tmp->value_folder.\n");
			exit (1);
		}
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
	folder = malloc(size + 1);
	if (!folder)
	{
		gc_cleanup(utils->shell->gcl);
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
