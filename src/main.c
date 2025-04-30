/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:45:28 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/28 17:54:14 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	prompt(void);
void	minishell(char *input);

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
//	expansao de variavel
// 	lexer
// 	parser
// 	executor
// 	garbage collector
void	minishell(char *input)
{
	t_token_list	*tokens;

	tokens = tokenizer(input);
}

//TODO: if (validar_input(input)) -- string vazia 
//TODO: minishell(input);
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
