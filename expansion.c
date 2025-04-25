/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:43:01 by marcudos          #+#    #+#             */
/*   Updated: 2025/04/24 17:33:12 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/include/boolean.h"
#include "libft/include/ft_string.h"
#include "minishell.h"

char	*expander(char *input, t_env *env)
{
	char	*res;
	char	*temp;
	t_quote	state;
	int		i;

	if (!input)
		return (NULL);
	res = ft_strdup("");
	i = 0;
	state = NO_QUOTE;
	while (input[i])
	{
		actualize_state_quote(input, &state, &i);
		temp = res;
		if (input[i] == '$' && state != SINGLE_QUOTE)
			res = ft_strjoin(temp, envp_expand(extract_key(&input[i + 1]),
						env));
		else
			res = ft_strjoin(temp, ft_substr(&input[i], 0, 1));
		free(temp);
		i += get_expand_len(&input[i], state);
	}
	return (res);
}

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
