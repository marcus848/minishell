/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 19:27:18 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/20 19:38:59 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_expander(t_exp *exp, char *input, t_env *env);
char	*ft_strjoin_free(char *s1, char *s2);
void	handle_next_token(t_exp *exp, char *input);

t_args	*expand_token(char *input, t_env *env)
{
	t_exp	exp;

	init_expander(&exp, input, env);
	while (input[exp.i])
		handle_next_token(&exp, input);
	if (exp.prefix)
		add_token(exp.head, exp.prefix);
	return (*exp.head);

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

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}
