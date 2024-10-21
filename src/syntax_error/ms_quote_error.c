/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_quote_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:02:50 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/21 18:18:33 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/syntax.h"

static void	st_args_verif(const char *input, t_shell *shell)
{
	int	error;

	error = 0;
	if (!input || !*input)
	{
		if (shell)
		{
			rl_clear_history();
			gc_cleanup(shell->gcl);
			free(shell->gcl);
		}
		error = 1;
	}
	if (!shell)
		error = 1;
	if (error)
	{
		perror("Minishell: Error: Segfault.\n");
		exit (EXIT_FAILURE);
	}
}

static void	st_assigned_quote(char current, char *target_quote, char *other_quote)
{
	if (current == '\'')
	{
		*target_quote = current;
		*other_quote = '\"';
	}
	else
	{
		*target_quote = current;
		*other_quote = '\'';
	}
}

static int	st_buff_between(char *current, char target, char other, t_shell *shell)
{
	int		len;
	char	*buffer;

	len = 0;
	while (current[len] || current[len] != target)
		len++;
	buffer = malloc(sizeof (len + 1));
	if (!buffer)
	{
		gc_cleanup(shell->gcl);
		free(shell->gcl);
		perror("Minishell: error: echec malloc\n");
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(buffer, current, len);
	buffer[len] = '\0';

	return (len);
}

static int	st_check_between_quote(char *current, t_shell *shell)
{
	int		i;
	char	target;
	char	other;

	i = 0;
	target = 0;
	other = 0;
	while (current[i])
	{
		if (current[i] == '\'' || current[i] == '\"')
		{
			st_assigned_quote(current[i], &target, &other);
			i += st_buff_between(current + i, target, other, shell);
		}
		else
			i++;
	}
	return (0);
}

int	ms_quote_error(const char *input, t_shell *shell)
{
	int		error;
	char	*current;

	st_args_verif(input, shell);
	current = (char *)input;
	error = 0;
	if (ft_strchr(current, '\'') || ft_strchr(current, '\"'))
	{
		error = st_check_between_quote(current, shell);
		if (error)
			printf("Minishell: syntax error: Not matching quote '\n");
	}
	return (error);
}
