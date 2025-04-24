/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 22:42:04 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/23 11:45:10 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/include/ft_string.h"
#include "minishell.h"
#include <stdbool.h>

//static int	handle_single_quoted(char *input, int *i, t_token_list *tokens);
/*
static int	handle_double_quoted(char *input, int *i, t_token_list *tokens);
static int	handle_operator(char *input, int *i, t_token_list *tokens);
static int	handle_word(char *input, int *i, t_token_list *tokens);
*/

/*
void	tokenizer(char *input)
{
	int	i;
	t_token_list	*tokens;

	i = 0;
	tokens = NULL;
	//token_list_init(&tokens);
	while (input[i])
	{
		if (ft_isspace(input[i]))
      		{
			i++;
			continue ;
		}
		if (handle_single_quoted(input, &i, tokens))
		{
			i++;
			continue ;
		}
		if (handle_double_quoted(input, &i, tokens))
		{
			i++;
			continue ;
		}
		if (handle_operator(input, &i, tokens))
			continue ;
		// fallback: plain word
		handle_word(input, &i, tokens);
	}
}

static int	handle_single_quoted(char *input, int *i, t_token_list *tokens)
{
	int	j;
	int	start;
	char	*value;
	t_token *tok;

	// initialize start to the dereference of i
	start = *i;
	// must start with a quote '
	if ((input[start]) != '\'')
		return (EXIT_FAILURE);
	// scan ahead for matching quote
	j = start + 1;
	while (input[j] && input[j] != '\'')
		j++;
	// if no closer found, don't consume, it's not a "quoted token"
	// from the MINISHELL SUBJECT:
	// 	"Your shell should:
	// 		- Not interpret unclosed quotes or special characters
	// 		which are not required by the subject such as \
	// 		(backslash) or ; (semicolon).
	if (input[j] != '\'')
		return (EXIT_FAILURE);
	// extract everything between the quotes
	value = ft_substr(input, start + 1, j - start -1);
	if (!value)
		exit_perror("ft_substr");
	// build and append a new WORD token
	tok = new_token(TOKEN_WORD, value);
	if (!tok)
	{
		free(value);
		exit_perror("new_token");
	}
	token_list_append(tokens, tok);
	// advance i past the coling quote
	*i = j;
	return (EXIT_SUCCESS);
}
*/
