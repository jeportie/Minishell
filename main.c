/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 11:52:47 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/31 13:56:27 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"
#include "include/tokenize.h"
#include "include/ast.h"

void	run(t_shell *shell, t_token *tokens, t_ast_node *root)
{
	tokens = ms_tokenize(shell->user_input, shell->gcl);
	print_token(tokens);
	printf("\n");
	root = ms_parse_tokens(tokens, shell->gcl);
	if (!root)
	{
		gc_cleanup(shell->gcl);
		exit(EXIT_FAILURE);
	}
	print_ast(root, 0, "", 0);
	printf("\n");
	printf("\n");
	shell->error_code = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	t_token		*tokens;
	t_ast_node	*root;

	shell = ms_init_shell(argc, argv, envp);
	tokens = NULL;
	root = NULL;
	gc_collect(shell.gcl);
	while (1)
	{
		ms_init_std_signal();
		ms_get_user_input(&shell);
		if (!shell.user_input)
		{
			rl_clear_history();
			gc_cleanup(shell.gcl);
			free(shell.gcl);
			exit (shell.error_code);
		}
		run(&shell, tokens, root);
	}
	shell.error_code = 666;
	return (shell.error_code);
}
