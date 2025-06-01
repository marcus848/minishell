/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wild_split.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 16:12:32 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/01 17:56:07 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
				add_token_to_array(&parts,
					remove_quotes(ft_substr(input, j, i - j), 1));
			j = i + 1;
		}
		i++;
	}
	if (i - j > 0)
		add_token_to_array(&parts,
			remove_quotes(ft_substr(input, j, i - j), 1));
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
	*array = new_array;
}

char	*remove_quotes(char *str, int free_str)
{
	char	*result;
	int		i;
	int		j;
	int		quote_handled;
	t_quote	state;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	state = NO_QUOTE;
	while (str[i])
	{
		quote_handled = update_state_quote_update_i(str, &i, &state);
		if (quote_handled)
			continue ;
		if (!((str[i] == '\'' && state == NO_QUOTE)
				|| (str[i] == '\"' && state == NO_QUOTE)))
			result[j++] = str[i];
		if (str[i])
			i++;
	}
	if (free_str)
		free(str);
	result[j] = '\0';
	return (result);
}
