/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 11:23:54 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/22 11:03:01 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "ast.h"
# include "minishell.h"
# include "process.h"
# include "exec.h"
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h>

typedef struct s_expand_utils
{
	char	*new_arg;
	char	*var;
	char	*expand_var;
	char	*start_var;
	int		total_len;
}				t_expand_utils;

typedef struct s_wildcard_context
{
	const char	*pattern;
	char		**matches;
	size_t		capacity;
	size_t		match_count;
	t_gc		*gcl;
}				t_wildcard_context;

t_wildcard_context	*ms_expand_wild(const char *pattern, t_gc *gcl);
int					initialize_context(t_wildcard_context *ctx,
						const char *pattern, t_gc *gcl);
int					add_match(t_wildcard_context *ctx, const char *filename);
int					check_match(t_wildcard_context *ctx, struct dirent *entry);
char				**finalize_matches(t_wildcard_context *ctx);

bool				is_var(char *cmd);
bool				is_wild(char *cmd);
bool				is_equal(char *cmd);
int					valide_dollar(char *input);

char				*find_dollar(char *arg);
int					ms_value_assign(t_shell *shell, t_cmd_node *cmd_node,
						t_gc *gcl);
char				*ms_expand_arg(char *arg, t_shell *shell, bool is_nested);
char				*nested_vars(char *arg, t_shell *shell);

void				ms_manage_arg_expand(t_cmd_node *cmd_node,
						t_exec_context *context, t_gc *gcl);

#endif /* EXPAND_H */
