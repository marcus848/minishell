/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:27:18 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/20 21:46:50 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_args	*expand_token(char *input, t_env *env)
{
	t_exp	exp;
	t_args	*result;

	init_expander(&exp, input, env);
	while (input[exp.i])
		handle_next_token(&exp, input);
	if (exp.prefix)
		add_token(exp.head, exp.prefix);
	result = *exp.head;
	free(exp.head);
	if (exp.prefix)
		free (exp.prefix);
	return (result);
}

void	handle_next_token(t_exp *exp, char *input)
{
	char	*key;

	if (input[exp->i] == '$')
	{
		key = extract_key(input, &exp->i);
		if (key)
		{
			expand_variable(exp, key);
			free(key);
		}
	}
	else
	{
		exp->cur = start_prefix(input, &exp->i, &exp->state);
		exp->prefix = ft_strjoin_free(exp->prefix, exp->cur);
		exp->cur = NULL;
	}
}

void	init_expander(t_exp *exp, char *input, t_env *env)
{
	exp->i = 0;
	exp->state = NO_QUOTE;
	exp->head = (t_args **) malloc(sizeof(t_args *));
	*exp->head = NULL;
	exp->prefix = start_prefix(input, &exp->i, &exp->state);
	exp->cur = NULL;
	exp->env = env;
}
