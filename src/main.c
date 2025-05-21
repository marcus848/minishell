/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:45:28 by caide-so          #+#    #+#             */
/*   Updated: 2025/05/20 21:24:11 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	prompt(t_env *env);
void	minishell(char *input, t_env *env);
void	parser(t_token_list *tokens, t_env *env);

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	prompt(env);
	clean_all(NULL, NULL, env);
	return (0);
}

void	minishell(char *input, t_env *env)
{
	t_token_list	*tokens;

	tokens = tokenizer(input);
	if (tokens == NULL)
		return ;
	parser(tokens, env);
}

void	parser(t_token_list *tokens, t_env *env)
{
	t_ast	*ast;

	if (!syntax_analysis(tokens))
		token_list_free(tokens);
	else
	{
		ast = parse_logical(&tokens->head);
		executor(tokens, ast, env);
		token_list_free(tokens);
		ast_free(ast);
	}
}

void	prompt(t_env *env)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (input[0] == '\0')
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
		minishell(input, env);
		free(input);
	}
}
