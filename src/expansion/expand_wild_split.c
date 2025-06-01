/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wild_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:12:32 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/01 16:46:28 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_token_to_array(char ***array, char *token);

char	**split_wildcard(char *input)
{
	t_quote	state;
	char	**parts;
	char	*start;
	int		i;
	int		j;

	state = NO_QUOTE;
	parts = NULL;
	start = input;
	i = 0;
	j = 0;
	while (input [i])
	{
		update_state_quote(&state, input[i]);
		if (input[i] == '*' && state == NO_QUOTE)
		{
			if (i - j > 0)
				add_token_to_array(&parts, remove_quotes(ft_substr(input, j, i - j)));
			j = i + 1;
		}
		i++;
	}
	if (i - j > 0)
		add_token_to_array(&parts, ft_substr(input, j, i = j));
	else if (j == 0)
		add_token_to_array(&parts, ft_strdup(""));
	return (parts);
}

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
	// free_array((void **) *array);
	*array = new_array;
}

char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	t_quote	state;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	state = NO_QUOTE;
	while (str[i])
	{
		update_state_quote_update_i(str, &i,  &state);
		if (!((str[i] == '\'' && state == NO_QUOTE) || (str[i] == '\"' && state == NO_QUOTE)))
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

