/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:10:11 by marcudos          #+#    #+#             */
/*   Updated: 2025/06/01 18:04:54 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Extracts an environment variable key starting at '$'.
// 1. Starts after '$' and reads alphanumeric or '_' characters.
// 2. Returns a new string with the extracted key.
char	*extract_key(char *input, int *i)
{
	int		start;

	if (input[*i] != '$')
		return (NULL);
	(*i)++;
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

// Updates the current quote state based on the input character.
// Handles transitions between NO_QUOTE, SINGLE_QUOTE, and DOUBLE_QUOTE.
void	update_state_quote(t_quote *state, char c)
{
	if ((c == '\'' && *state == SINGLE_QUOTE)
		|| (c == '\"' && *state == DOUBLE_QUOTE))
		*state = NO_QUOTE;
	else if ((c == '\'' && *state == NO_QUOTE))
		*state = SINGLE_QUOTE;
	else if ((c == '\"' && *state == NO_QUOTE))
		*state = DOUBLE_QUOTE;
}

// Finds the value for a given environment variable key.
// 1. Iterates through the env list.
// 2. Returns a strdup of the value if found, or an empty string otherwise.
char	*find_env_value(char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(key, env->key) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

// Joins two strings and frees both.
// 1. Handles NULL safely.
// 2. Frees both inputs regardless of which one is NULL.
// 3. Returns the newly joined string.
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
	{
		res = ft_strdup(s2);
		free(s2);
		return (res);
	}
	else if (!s2)
	{
		res = ft_strdup(s1);
		free(s1);
		return (res);
	}
	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}
