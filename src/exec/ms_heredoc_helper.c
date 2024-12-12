/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:08:10 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/12 17:31:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/expand.h"

/*int	ft_check_delim(char *line, char *delimiter)
{
	int	i;

	i = 0;
	while (line[i] && delimiter[i])
	{
		if (line[i] == delimiter[i])
			i++;
		else
			return (1);
	}
	if (line[i] == '\n' && !delimiter[i])
		return (0);
	else
		return (1);
}

char	*ft_make_delim(t_infos *infos, char *buff)
{
	int	i;
	int	verif;

	i = 0;
	verif = 0;
	while (buff[i])
	{
		if (ft_isalpha(buff[i]))
			verif++;
		i++;
	}
	if (verif > 0)
		return (ft_strdup(buff));
	else if (!infos->error)
	{
		ft_dprintf(2, LOG_DEL, buff, "\'\n");
		infos->error = "\0";
	}
	return (ft_strdup(buff));
}

void	ft_clear_heredoc_buffer(t_tokenizer *tok)
{
	int	i;

	i = 0;
	tok->i -= ft_strlen(tok->buffer);
	while (i < (int)ft_strlen(tok->buffer))
	{
		tok->buffer[i] = '\0';
		tok->i++;
		i++;
	}
}

void	ft_extract_heredoc_delimiter(t_tokenizer *tok)
{
	tok->j = 0;
	while (tok->input[tok->i] == '<' || tok->input[tok->i] == ' '
		|| tok->input[tok->i] == '>' || tok->input[tok->i] == '|')
		tok->i++;
	while (tok->input[tok->i] && tok->input[tok->i] != ' '
		&& tok->input[tok->i] != '<' && tok->input[tok->i] != '>'
		&& tok->input[tok->i] != '|')
		tok->buffer[tok->j++] = tok->input[tok->i++];
	tok->buffer[tok->j] = '\0';
	tok->i--;
}*/
