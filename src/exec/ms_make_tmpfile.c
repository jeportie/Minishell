/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_make_tmpfile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:40:27 by jeportie          #+#    #+#             */
/*   Updated: 2024/12/12 18:10:19 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

static void	ft_give_index(int n, char *str)
{
	if (n > 99)
	{
		str[3] = '\0';
		str[2] = n % 10 + '0';
		n /= 10;
		str[1] = n + '0';
		n /= 10;
		str[0] = n + '0';
	}
	else if (n > 9)
	{
		str[2] = '\0';
		str[1] = n % 10 + '0';
		n /= 10;
		str[0] = n + '0';
	}
	else
	{
		str[0] = n + '0';
		str[1] = '\0';
	}
}

void	ft_generate_temp_filename(char *buffer, size_t buffer_size,
	t_exec_context *context)
{
	char		counter_str[4];
	const char	*prefix = "~/tmp/.ms_tmpfile_";
	size_t		prefix_len;
	size_t		counter_len;

	prefix_len = ft_strlen(prefix);
	if (prefix_len >= buffer_size)
		return ;
	ft_strlcpy(buffer, prefix, buffer_size);
	ft_give_index(context->tmpfile_counter, counter_str);
	counter_len = ft_strlen(counter_str);
	if (prefix_len + counter_len + 1 >= buffer_size)
		return ;
	ft_strlcpy(buffer + prefix_len, counter_str, buffer_size - prefix_len);
	if (access(buffer, F_OK) != 0)
	{
		context->tmpfile_counter++;
		ft_memset(buffer, 0, 260 * sizeof(char));
		return (ft_generate_temp_filename(buffer, sizeof(buffer), context));
	}
}

/*void	ft_generate(t_infos *infos, t_tokenizer *tok, char *tmp_filename,
	char *delimiter)
{
	int	fd;

	fd = open(tmp_filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
		ft_quit(infos, 2, "Error: echec open fd.\n", errno);
	write(fd, tok->heredoc_buffer, ft_strlen(tok->heredoc_buffer));
	close(fd);
	delimiter = ft_free_str(delimiter);
	tok->buffer = ft_free_str(tok->buffer);
	tok->heredoc_buffer = ft_free_str(tok->heredoc_buffer);
	ft_free_infos(infos);
}

t_files	*ft_init_file(t_infos *infos, int type, char *file)
{
	t_files	*new;

	new = malloc(sizeof(t_files));
	if (!new)
		ms_quit(infos, 2, "Error: echec malloc new.\n", errno);
	new->type = type;
	new->name = ft_strdup(file);
	if (!new->name)
		ft_quit(infos, 2, "Error: echec malloc new->file.\n", errno);
	new->next = NULL;
	return (new);
}

void	ft_add_file(t_infos *infos, t_tok *current_tok,
	int type, char *file)
{
	t_files *(new_file) = NULL;
	t_files **(file_list) = NULL;
	t_files *(tmp) = NULL;
	if (!current_tok)
	{
		current_tok = ft_init_tok(infos);
		ft_add_cmd(infos, current_tok, NULL);
	}
	new_file = ft_init_file(infos, type, file);
	file_list = &current_tok->files;
	if (*file_list == NULL)
		*file_list = new_file;
	else
		ft_add_utils(tmp, file_list, new_file);
}*/
