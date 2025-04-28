/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 22:42:04 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/25 15:57:21 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_word(char *input, int *i, t_token_list *tokens);
static void	handle_quotes(char *input, int *j);

// TODO: testar essa frase - joao-"e"-'maria'""''se-"'aman'"'"alem"'-do-infinito

void	tokenizer(char *input)
{
	int				i;
	t_token_list	tokens;

	i = 0;
	token_list_init(&tokens);
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		if (handle_operators(input, &i, &tokens))
			continue ;
		handle_word(input, &i, &tokens);
	}
	print_tokens(&tokens);
}

static int	handle_word(char *input, int *i, t_token_list *tokens)
{
	int		j;
	int		start;
	char	*value;
	t_token	*token;

	start = *i;
	j = start;
	while (input[j] && !in("()&|<>*", input[j]) && !ft_isspace(input[j]))
		handle_quotes(input, &j);
	if (j == start)
		return (0);
	value = ft_substr(input, start, j - start);
	if (!value)
		exit_perror("ft_substr");
	token = new_token(TOKEN_WORD, value);
	if (!token)
	{
		free(value);
		exit_perror("new_token");
	}
	token_list_append(tokens, token);
	*i = j;
	return (1);
}

static void	handle_quotes(char *input, int *j)
{
	char	type;

	if (input[*j] == '\'' || input[*j] == '\"')
	{
		type = input[*j];
		(*j)++;
		while (input[*j] && input[*j] != type)
			(*j)++;
		if (input[*j] == type)
			(*j)++;
	}
	else
		(*j)++;
}
