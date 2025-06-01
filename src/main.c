/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:45:28 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/23 03:39:38 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	prompt(t_shell *shell);
void	minishell(char *input, t_shell *shell);
void	parser(t_token_list *tokens, t_shell *shell);

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.env = init_env(envp);
	shell.tokens = NULL;
	shell.ast = NULL;
	shell.last_status = 0;
	prompt(&shell);
	clean_all(NULL, NULL, &shell.env);
	return (0);
}

void	prompt(t_shell *shell)
{
	char	*input;
	char	*pmt;

	while (1)
	{
		pmt = make_prompt(shell);
		input = readline(pmt);
		free(pmt);
		if (!input)
			break ;
		if (input[0] == '\0' || is_all_whitespace(input))
		{
			free(input);
			continue ;
		}
		if (ft_strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break ;
		}
		add_history(input);
		minishell(input, shell);
		free(input);
	}
}

void	minishell(char *input, t_shell *shell)
{
	t_token_list	*tokens;

	tokens = tokenizer(input);
	if (!tokens)
		return ;
	shell->tokens = tokens;
	parser(tokens, shell);
}

void	parser(t_token_list *tokens, t_shell *shell)
{
	t_ast	*ast;

	if (!syntax_analysis(tokens))
	{
		token_list_free(tokens);
		return ;
	}
	ast = parse_logical(&tokens->head);
	shell->ast = ast;
	// executor(tokens, shell, ast);
	print_ast(ast, 0, shell->env, shell);
	token_list_free(tokens);
	ast_free(shell->ast);
	shell->ast = NULL;
	shell->tokens = NULL;
}
