/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wild_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:12:32 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/01 18:35:06 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Initializes variables used in split_wildcard().
void	init_vars_split_wildcard(t_quote *state, char ***parts, int *i, int *j)
{
	*state = NO_QUOTE;
	(*parts) = NULL;
	*i = 0;
	*j = 0;
}

// Splits a wildcard pattern string into parts separated by unquoted asterisk.
// 1. Initializes state and indices.
// 2. Iterates over the input string updating quote state.
// 3. On unquoted '*', extracts substring between indices and adds parts array 
// after removing quotes.
// 4. Adds final substring after loop if exists, or adds empty string if none
// found.
// 5. Returns array of substrings.
char	**split_wildcard(char *input)
{
	t_quote	state;
	char	**parts;
	int		i;
	int		j;

	init_vars_split_wildcard(&state, &parts, &i, &j);
	while (input [i])
	{
		update_state_quote(&state, input[i]);
		if (input[i] == '*' && state == NO_QUOTE)
		{
			if (i - j > 0)
				add_token_to_array(&parts,
					rem_quotes(ft_substr(input, j, i - j), 1));
			j = i + 1;
		}
		i++;
	}
	if (i - j > 0)
		add_token_to_array(&parts, rem_quotes(ft_substr(input, j, i - j), 1));
	else if (j == 0)
		add_token_to_array(&parts, ft_strdup(""));
	return (parts);
}

// Adds a new token string to a dinamically growing array of strings.
// 1. Calculates current length of the array.
// 2. Allocates a new array with space for the new token and NULL terminator.
// 3. Copies existing pointers to new array.
// 4. Adds new token pointer at the end and NULL terminator.
// 5. Frees old array and updates pointer to new array.
void	add_token_to_array(char ***array, char *token)
{
	char	**new_array;
	int		len;
	int		i;

	if (!token)
		return ;
	len = count_array((void **) *array);
	new_array = malloc(sizeof(char *) * (len + 2));
	if (!new_array)
	{
		free(token);
		return ;
	}
	i = 0;
	while (*array && (*array)[i])
	{
		new_array[i] = (*array)[i];
		i++;
	}
	new_array[i++] = token;
	new_array[i] = NULL;
	free(*array);
	*array = new_array;
}
