/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:45:28 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/23 11:43:08 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt(void);
void	minishell(char *input);

void	print_env(t_env *env);

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
/*
void	minishell(char *input)
{
//	expansao de variavel
// 	token
// 	lexer
// 	parser
// 	executor
// 	garbage collector
}
*/

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
			free(input);
		}
		else
			printf("Error reading input or end of file reached.\n");
	}
}

// ============================ DEBUG FUNCTIONS ===============================

/*
// Prints the full t_envp struct.
void	print_env(t_env *env)
{
	t_env	*head;

	head = env;
	while (head)
	{
		printf("key = [%s], value = [%s]\n", head->key, head->value);
		head = head->next;
	}
}
*/
