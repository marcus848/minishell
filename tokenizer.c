/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 22:42:04 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/24 22:18:15 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quotes(char *input, int *i, char type, t_token_list *tokens);
/*
static int	handle_operator(char *input, int *i, t_token_list *tokens);
static int	handle_word(char *input, int *i, t_token_list *tokens);
*/
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
		if (handle_quotes(input, &i, '\'', &tokens))
			continue ;
		if (handle_quotes(input, &i, '\"', &tokens))
			continue ;
		/*
		if (handle_operator(input, &i, &tokens))
			continue ;
		// fallback: plain word
		handle_word(input, &i, &tokens);
		*/
	}
}

// Handles single and double quoted sections of the input
// 1. Initialize start to the dereference of i.
// 2. type parameter is ' or ".
// Note: Must start with a quote.
// 3. Scan ahead for matching quote.
// Note: If no closer found, don't consume, it's not a "quoted token".
// Note. From the MINISHELL SUBJECT:
// "Your shell should:
// - Not interpret unclosed quotes or special characters which are not
// required by the subject such as \ (backslash) or ; (semicolon).
// 4. Extract everything between the quotes.
// 5. Build and append a new WORD token.
// 6. Advance i past the closing quote
static int	handle_quotes(char *input, int *i, char type, t_token_list *tokens)
{
	int		j;
	int		start;
	char	*value;
	t_token	*tok;

	start = *i;
	if ((input[start]) != type)
		return (0);
	j = start + 1;
	while (input[j] && input[j] != type)
		j++;
	if (input[j] != type)
		return (0);
	value = ft_substr(input, start + 1, j - start -1);
	if (!value)
		exit_perror("ft_substr");
	tok = new_token(TOKEN_WORD, value);
	if (!tok)
	{
		free(value);
		exit_perror("new_token");
	}
	token_list_append(tokens, tok);
	*i = j + 1;
	return (1);
}

/*
static int	handle_operator(char *input, int *i, t_token_list *tokens)
{
	int	start;

	start = *i;
	if (input[start] != '&')
		return (0);
}
*/
