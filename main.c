/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:52:47 by jeportie          #+#    #+#             */
/*   Updated: 2025/01/07 14:05:53 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"
#include "include/syntax.h"
#include "include/exec.h"
#include "lib/libft/include/libft.h"

static void	st_delete_tmp_files(void)
{
	char	filepath[32];

	int (n) = 0;
	while (n <= 255)
	{
		ft_snprintf(filepath, sizeof(filepath), "/tmp/.ms_tmpfile_%d", n);
		if (access(filepath, F_OK) == 0)
		{
			if (unlink(filepath) != 0)
				ft_dprintf(2, "Failed to delete tmpfile.\n");
		}
		n++;
	}
}

static void	run(t_shell *shell, t_token *tokens, t_ast_node *root)
{
	t_exec_context	context;

	tokens = ms_tokenize(shell->user_input, shell->gcl);
	print_token_delimit(tokens);
	root = ms_parse_tokens(tokens, shell, shell->gcl);
	if (!root)
		return ;
	print_ast_delimit(root);
	gc_collect(shell->gcl);
	init_context(&context, shell);
	ms_preprocess_heredocs(&context);
	shell->error_code = ms_execute_ast(root, &context);
	gc_collect(shell->gcl);
	st_delete_tmp_files();
}

static void	st_quit_prompt(t_shell *shell)
{
	rl_clear_history();
	gc_cleanup(shell->gcl);
	if (shell->interactive_mode)
		ft_dprintf(1, "exit\n");
	exit (shell->error_code);
}

int	main(int argc, char **argv, char **envp)
{
	t_token		*tokens;
	t_ast_node	*root;
	t_shell		shell;

	root = NULL;
	tokens = NULL;
	shell = ms_init_shell(argc, argv, envp);
	while (1)
	{
		ms_init_std_signal();
		ms_get_user_input(&shell);
		if (!shell.user_input)
			st_quit_prompt(&shell);
		if (ms_syntax_error(shell.user_input))
			shell.error_code = 2;
		else
			run(&shell, tokens, root);
		ft_bzero(shell.heredocs, 16);
	}
	return (0);
}
