/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:45:28 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/03 18:26:50 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	prompt(void);
void	minishell(char *input);
void	parser(t_token_list *tokens);

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	prompt();
	clean_all(env);
	return (0);
}

//TODO: tudo abaixo
// 	parser
// 	executor
// 	garbage collector
void	minishell(char *input)
{
	t_token_list	*tokens;

	tokens = tokenizer(input);
	test_commands_from_tokens(tokens);
	print_ast(parse_logical(&tokens->head), 0);
	token_list_free(tokens);
}

void	parser(t_token_list *tokens)
{
	(void)tokens;
	printf("oi eu sou o senhor parser\n");
}

void	prompt(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input || ft_strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break ;
		}
		else if (input != NULL)
		{
			add_history(input);
			minishell(input);
			free(input);
		}
		else
			printf("Error reading input or end of file reached.\n");
	}
}
