/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:55:40 by marcudos          #+#    #+#             */
/*   Updated: 2025/04/28 17:41:10 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_state_quote(char *input, t_quote *state, int *i)
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
	int	len;

	if (!input)
		return (1);
	len = 1;
	if (input[0] == '$' && state != SINGLE_QUOTE)
	{
		while (ft_isalnum(input[len]) || input[len] == '_')
			len++;
	}
	return (len);
}
