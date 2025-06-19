/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_handle_operators.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caide-so <caide-so@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:44:29 by caide-so          #+#    #+#             */
/*   Updated: 2025/04/30 20:04:30 by caide-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_two_char_op(char *input, t_token_type *type, int *len);
int	is_one_char_op_1(char input, t_token_type *type, int *len);
int	is_one_char_op_2(char input, t_token_type *type);
int	create_operator_token(char *input, int *i, t_token_list *tokens);

// Handles operator characters in input.
// 1. Checks if current character is an operator.
// 2. Creates operator token if valid.
// 3. Reports errors for invalid operators.
// Returns 1 if operator handled, 0 on error, -1 if not operator.
int	handle_operators(char *input, int *i, t_token_list *tokens)
{
	if (!in("()&|<>", input[*i]))
		return (-1);
	if (create_operator_token(input, i, tokens))
		return (1);
	if (input[*i] == '&')
	{
		report_unexpected("&");
		return (0);
	}
	return (-1);
}

// Creates and appends operator token to list.
// 1. Determines operator type and length.
// 2. Extracts operator substring.
// 3. Creates and appends new token.
// Returns 1 on success, 0 if no operator found.
int	create_operator_token(char *input, int *i, t_token_list *tokens)
{
	int				len;
	char			*value;
	t_token			*token;
	t_token_type	type;

	if (is_two_char_op(input + *i, &type, &len)
		|| is_one_char_op_1(input[*i], &type, &len))
	{
		value = ft_substr(input, *i, len);
		if (!value)
			exit_perror("ft_substr");
		token = new_token(type, value);
		if (!token)
		{
			free(value);
			exit_perror("new_token");
		}
		token_list_append(tokens, token);
		*i += len;
		return (1);
	}
	return (0);
}

// Checks for two-character operators.
// 1. Tests for >>, <<, && and ||.
// 2. Sets type and length if found.
// Returns 1 if match found, 0 otherwise.
int	is_two_char_op(char *input, t_token_type *type, int *len)
{
	*len = 2;
	if (input[0] == '>' && input[1] == '>')
	{
		*type = REDIR_APPEND;
		return (1);
	}
	else if (input[0] == '<' && input[1] == '<')
	{
		*type = HEREDOC;
		return (1);
	}
	else if (input[0] == '&' && input[1] == '&')
	{
		*type = LOGICAL_AND;
		return (1);
	}
	else if (input[0] == '|' && input[1] == '|')
	{
		*type = LOGICAL_OR;
		return (1);
	}
	return (0);
}

// Checks for single-character operators (part 1).
// 1. Tests for >, <, |.
// 2. Sets type and length if found.
// 3. Delegates remaining checks to is_one_char_op_2
// Returns 1 if match found, 0 otherwise.
int	is_one_char_op_1(char input, t_token_type *type, int *len)
{
	*len = 1;
	if (input == '>')
	{
		*type = REDIR_OUT;
		return (1);
	}
	else if (input == '<')
	{
		*type = REDIR_IN;
		return (1);
	}
	else if (input == '|')
	{
		*type = PIPE;
		return (1);
	}
	return (is_one_char_op_2(input, type));
}

// Checks for single-character operators (part 1).
// 1. Tests for (, ).
// 2. Sets type if found.
// Returns 1 if match found, 0 otherwise.
int	is_one_char_op_2(char input, t_token_type *type)
{
	if (input == '(')
	{
		*type = PAREN_OPEN;
		return (1);
	}
	else if (input == ')')
	{
		*type = PAREN_CLOSE;
		return (1);
	}
	return (0);
}
