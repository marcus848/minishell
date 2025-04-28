/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:43:01 by marcudos          #+#    #+#             */
/*   Updated: 2025/04/28 17:52:00 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/include/ft_string.h"
#include "minishell.h"

void	expander(char	***args, t_env *env)
{
	char	*expanded;
	char	*temp;
	int		i;
	
	i = 0;
	while ((*args)[i])
	{
		expanded = expander_expand((*args)[i], env);
		temp = (*args)[i];
		(*args)[i] = expanded;
		free(temp);
		i++;
	}
}

char	*expander_expand(char *input, t_env *env)
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
		res = ft_strjoin(temp, expand_token(&input[i], env, state));
		free(temp);
		i += get_expand_len(&input[i], state);
	}
	return (res);
}

char	*expand_token(char *input, t_env *env, t_quote state)
{
	if (input[0] == '$' && state != SINGLE_QUOTE)
	{
		return (expand_env(&input[1], env));
	}
	else if (input[0] == '~')
		return (expand_env("HOME", env));
	return (ft_substr(input, 0, 1));
}
