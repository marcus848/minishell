/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcudos <marcudos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:22:33 by marcudos          #+#    #+#             */
/*   Updated: 2025/05/20 19:34:37 by marcudos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_args	*expand_env(char **args, t_env *env)
{
	t_args	*head;
	t_args	*cur;
	t_args	*new;
	int	i;

	if (!args || !args[0])
		return (NULL);
	head = expand_token(args[0], env);
	cur = head;
	i = 1;
	while (args[i])
	{
		new = expand_token(args[i], env);
		while (cur->next)
			cur = cur->next;
		cur->next = new;
		i++;
	}
	return (head);
}

// t_args	*expand_token(char *input, t_env *env)
// {
// 	char	*key;
// 	int	i;
// 	t_exp	exp;
//
// 	i = 0;
// 	exp.state = NO_QUOTE; 
// 	exp.head = NULL;
// 	exp.env = env;
// 	exp.head = (t_args **) malloc(sizeof(t_args *));
// 	*exp.head = NULL;
// 	exp.prefix = NULL;
// 	exp.prefix = start_prefix(input, &i, &exp.state);
// 	while (input[i])
// 	{
// 		if (input[i] == '$')
// 		{
// 			key = extract_key(input, &i);
// 			if (key)
// 				expand_variable(&exp, key);
// 		}
// 		else
// 		{
// 			exp.cur = start_prefix(input, &i, &exp.state);
// 			exp.prefix = ft_strjoin(exp.prefix, exp.cur);
// 			exp.cur = NULL;
// 		}
// 	}
// 	add_token(exp.head, exp.prefix);
// 	return ((*exp.head));
// }


char	*start_prefix(char *input, int *i, t_quote *state)
{
	char	*prefix;
	char	*temp;
	char	*new;

	prefix = NULL;
	while (input[*i])
	{
		update_state_quote(input, i, state);
		if (input[*i] == '$' && *state != SINGLE_QUOTE)
			break ;
		temp = ft_substr(input, *i, 1);
		if (prefix)
		{
			new = ft_strjoin(prefix, temp);
			free(prefix);
			free(temp);
			prefix = new;
		}
		else
			prefix = temp;
		(*i)++;
	}
	return (prefix);
}

void	expand_variable(t_exp *exp, char *key)
{
	char	*temp;
	char	**parts;
	int	i;

	exp->cur = find_env_value(key, exp->env);
	if (!exp->cur)
		return ;
	if (!exp->prefix)
		exp->prefix = ft_strdup("");
	temp = exp->prefix;
	exp->prefix = ft_strjoin(temp, exp->cur);
	free(temp);
	free(exp->cur);
	exp->cur = NULL;
	if (exp->state == NO_QUOTE)
	{
		parts = ft_split(exp->prefix, ' ');
		if (count_array((void **) parts) == 1)
			exp->prefix = ft_strdup(parts[0]);
		else
		{
			i = 0;
			while (i == 0 || (parts[i] && parts[i + 1]))
				add_token(exp->head, parts[i++]);
			if (!parts[i])
				exp->prefix = NULL;
			else
				exp->prefix = ft_strdup(parts[i]);
		}
		free(parts);
	}
}

