/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:55:40 by marcudos          #+#    #+#             */
/*   Updated: 2025/04/28 16:22:34 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	actualize_state_quote(char *input, t_quote *state, int *i)
{
	while (input[*i] == '\'' || input[*i] == '\"')
	{
		if ((input[*i] == '\'' && *state == SINGLE_QUOTE)
			|| (input[*i] == '\"' && *state == DOUBLE_QUOTE))
		{
			*state = NO_QUOTE;
			(*i)++;
		}
		else if (input[*i] == '\'' && *state == NO_QUOTE)
		{
			*state = SINGLE_QUOTE;
			(*i)++;
		}
		else if (input[*i] == '\"' && *state == NO_QUOTE)
		{
			*state = DOUBLE_QUOTE;
			(*i)++;
		}
		else
			break ;
	}
}

char	*envp_expand(char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

char	*extract_key(char *input)
{
	int	i;

	i = 0;
	while ((ft_isalnum(input[i]) || input[i] == '_') && input[i])
		i++;
	return (ft_substr(input, 0, i));
}

int	get_expand_len(char *input, t_quote state)
{
	char	*key;
	int		len;

	if (input[0] != '$' || state == SINGLE_QUOTE)
		return (1);
	key = extract_key(&input[1]);
	len = 1 + ft_strlen(key);
	free(key);
	return (len);
}
